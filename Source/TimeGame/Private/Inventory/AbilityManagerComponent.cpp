// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/AbilityManagerComponent.h"

#include "AbilitySystemComponent.h"
#include "Core/TgPlayerCharacter.h"

FAbilityBinding::FAbilityBinding(): InputAction(nullptr), ActivationType()
{
}

bool FAbilityBinding::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	Ar << AbilityClass;
	UObject* ActionToSerialize = InputAction;
	Map->SerializeObject(Ar, UInputAction::StaticClass(), ActionToSerialize);
	InputAction = Cast<UInputAction>(ActionToSerialize);
	Ar << ActivationType;
	return bOutSuccess;
}

UAbilityManagerComponent::UAbilityManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicated(true);
}

void UAbilityManagerComponent::GrantAndBindActiveAbility(const TSubclassOf<UGameplayAbility>& Ability,
                                                         UInputAction* InputAction,
                                                         const EInputActionActivationType ActivationType)
{
	if (!GetOwner()->HasAuthority()) return;
	if (!Ability.Get()) return;
	UAbilitySystemComponent* Asc = GetAsc();
	if (!Asc) return;

	if (ServerGrantedAbilities.Contains(Ability))
	{
		bRejectNextAbilityRemoval = true;
		return;
	}
	
	ServerGrantedAbilities.Add(Ability, Asc->GiveAbility(FGameplayAbilitySpec(Ability)));

	FAbilityBinding Binding;
	Binding.AbilityClass = Ability;
	Binding.InputAction = InputAction;
	Binding.ActivationType = ActivationType;
	AbilityBindings.Add(Binding);

	UpdateAbilityBindings(AbilityBindings);
}

void UAbilityManagerComponent::RemoveActiveAbility(const TSubclassOf<UGameplayAbility>& Ability)
{
	if (!GetOwner()->HasAuthority()) return;
	if (!Ability.Get()) return;
	UAbilitySystemComponent* Asc = GetAsc();
	if (!Asc) return;

	if (bRejectNextAbilityRemoval)
	{
		bRejectNextAbilityRemoval = false;
		return;
	}
	
	if (!ServerGrantedAbilities.Contains(Ability)) return;

	Asc->ClearAbility(ServerGrantedAbilities[Ability]);
	ServerGrantedAbilities.Remove(Ability);

	for (int16 i = AbilityBindings.Num() - 1; i >= 0; i--)
	{
		if (AbilityBindings[i].AbilityClass == Ability)
		{
			AbilityBindings.RemoveAt(i);
			break;
		}
	}

	UpdateAbilityBindings(AbilityBindings);
}

float UAbilityManagerComponent::GetCooldown(const TSubclassOf<UGameplayAbility>& Ability) const
{
	UAbilitySystemComponent* Asc = GetAsc();
	if (!Asc) return 0;

	const FGameplayAbilitySpec* Spec = Asc->FindAbilitySpecFromClass(Ability);
	if (!Spec) return 0;
	const UGameplayAbility* PrimaryAbilityInstance = Spec->GetPrimaryInstance();
	if (!PrimaryAbilityInstance) return 0;
	return PrimaryAbilityInstance->GetCooldownTimeRemaining();
}

void UAbilityManagerComponent::OnInput(const UInputAction* InputAction, const EInputActionActivationType ActivationType)
{
	for (const FAbilityBinding& Binding : AbilityBindings)
	{
		if (Binding.InputAction == InputAction && Binding.ActivationType == ActivationType)
		{
			UAbilitySystemComponent* Asc = GetAsc();
			if (!Asc) return;
			Asc->TryActivateAbilityByClass(Binding.AbilityClass);
			return;
		}
	}
}

void UAbilityManagerComponent::GrantPassiveAbility(const TSubclassOf<UGameplayAbility>& Ability)
{
	if (!GetOwner()->HasAuthority()) return;
	if (!Ability.Get()) return;
	UAbilitySystemComponent* Asc = GetAsc();
	if (!Asc) return;

	if (ServerGrantedAbilities.Contains(Ability))
	{
		bRejectNextAbilityRemoval = true;
		return;
	}

	ServerGrantedAbilities.Add(Ability, Asc->GiveAbility(FGameplayAbilitySpec(Ability)));
	Asc->TryActivateAbilityByClass(Ability);
}

void UAbilityManagerComponent::RemovePassiveAbility(const TSubclassOf<UGameplayAbility>& Ability)
{
	if (!GetOwner()->HasAuthority()) return;
	if (!Ability.Get()) return;
	UAbilitySystemComponent* Asc = GetAsc();
	if (!Asc) return;

	if (bRejectNextAbilityRemoval)
	{
		bRejectNextAbilityRemoval = false;
		return;
	}
	
	if (!ServerGrantedAbilities.Contains(Ability)) return;

	Asc->CancelAbilityHandle(ServerGrantedAbilities[Ability]);
	Asc->ClearAbility(ServerGrantedAbilities[Ability]);
	ServerGrantedAbilities.Remove(Ability);
}

void UAbilityManagerComponent::UpdateAbilityBindings_Implementation(const TArray<FAbilityBinding>& InAbilityBindings)
{
	AbilityBindings = InAbilityBindings;
}

void UAbilityManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<ATgPlayerCharacter>(GetOwner());
}

UAbilitySystemComponent* UAbilityManagerComponent::GetAsc() const
{
	if (!PlayerCharacter || !PlayerCharacter->GetAbilitySystemComponent())
	{
		return nullptr;
	}
	return PlayerCharacter->GetAbilitySystemComponent();
}

void UAbilityManagerComponent::GrantInfiniteEffect(const TSubclassOf<UGameplayEffect>& Effect)
{
	if (!GetOwner()->HasAuthority()) return;
	if (!Effect.Get()) return;
	UAbilitySystemComponent* Asc = GetAsc();
	if (!Asc) return;

	if (ServerGrantedEffects.Contains(Effect))
	{
		RejectNextEffectRemovals++;
		return;
	}

	ServerGrantedEffects.Add(Effect, Asc->ApplyGameplayEffectToSelf(Effect.GetDefaultObject(), 1, Asc->MakeEffectContext()));
}

void UAbilityManagerComponent::RemoveInfiniteEffect(const TSubclassOf<UGameplayEffect>& Effect)
{
	if (!GetOwner()->HasAuthority()) return;
	if (!Effect.Get()) return;
	UAbilitySystemComponent* Asc = GetAsc();
	if (!Asc) return;

	if (RejectNextEffectRemovals > 0)
	{
		RejectNextEffectRemovals--;
		return;
	}
	
	if (!ServerGrantedEffects.Contains(Effect)) return;

	Asc->RemoveActiveGameplayEffect(ServerGrantedEffects[Effect]);
	ServerGrantedEffects.Remove(Effect);
}