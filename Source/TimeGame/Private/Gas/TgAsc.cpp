// Fill out your copyright notice in the Description page of Project Settings.

#include "TimeGame/Public/Gas/TgAsc.h"

#include "Core/TgPlayerCharacter.h"

UTgAsc::UTgAsc()
{
}

void UTgAsc::BeginPlay()
{
	Super::BeginPlay();

	if (!GetOwner()->HasAuthority()) return;
	OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(this, &UTgAsc::OnServerActiveGameplayEffectAdded);
}

void UTgAsc::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UTgAsc::BroadcastReceiveDamage(UAbilitySystemComponent* Source, const float ProcessedShieldDamage,
	const float ProcessedHealthDamage, const float OriginalDamage)
{
	if (OnReceiveDamage.IsBound())
	{
		OnReceiveDamage.Broadcast(Source, ProcessedShieldDamage, ProcessedHealthDamage, OriginalDamage);
	}
}

void UTgAsc::BroadcastReceiveHealing(UAbilitySystemComponent* Source, const float ProcessedHealing,
	const float OriginalHealing)
{
	if (OnReceiveHealing.IsBound())
	{
		OnReceiveHealing.Broadcast(Source, ProcessedHealing, OriginalHealing);
	}
}

void UTgAsc::PerformOnHit(UAbilitySystemComponent* Target)
{
	if (OnHitRegistry.IsBound())
	{
		OnHitRegistry.Broadcast(this, Target);
	}
}

void UTgAsc::OnServerActiveGameplayEffectAdded(UAbilitySystemComponent* Asc, const FGameplayEffectSpec& Spec,
	FActiveGameplayEffectHandle EffectHandle) const
{
	if (!Spec.Def) return;
	if (Spec.Def->StackingType != EGameplayEffectStackingType::None) return;

	FGameplayEffectQuery Query = FGameplayEffectQuery();
	Query.EffectDefinition = Spec.Def.GetClass();
	
	TArray<FActiveGameplayEffectHandle> FoundEffects = Asc->GetActiveEffects(Query);
	if (FoundEffects.Num() <= 1) return;
	if (!EffectSelectionAttribute.IsValid())
	{
		UE_LOG(LogTimeGame, Error, TEXT("EffectSelectionAttribute must be set in the TgAsc."));
		return;
	}
	FGameplayAttribute ScopedEffectSelectionAttribute = EffectSelectionAttribute;
	FoundEffects.Sort([Asc, ScopedEffectSelectionAttribute](const FActiveGameplayEffectHandle& A, const FActiveGameplayEffectHandle& B)
	{
		//Compare the attribute given by the editor.
		const float ValueA = Asc->GetActiveGameplayEffect(A)->Spec.GetContext().GetOriginalInstigatorAbilitySystemComponent()->GetNumericAttribute(ScopedEffectSelectionAttribute);
		const float ValueB = Asc->GetActiveGameplayEffect(B)->Spec.GetContext().GetOriginalInstigatorAbilitySystemComponent()->GetNumericAttribute(ScopedEffectSelectionAttribute);
		return ValueA < ValueB;
	});
	FoundEffects.RemoveAt(FoundEffects.Num() - 1, 1, false);
	for (const FActiveGameplayEffectHandle& FoundEffectHandle : FoundEffects)
	{
		Asc->RemoveActiveGameplayEffect(FoundEffectHandle);
	}
}

