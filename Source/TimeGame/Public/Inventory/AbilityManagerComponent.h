// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "InputAction.h"
#include "Abilities/GameplayAbility.h"
#include "Components/ActorComponent.h"
#include "AbilityManagerComponent.generated.h"

class ATgPlayerCharacter;

UENUM(BlueprintType)
enum class EInputActionActivationType : uint8
{
	Down,
	Up
};

USTRUCT(BlueprintType)
struct FAbilityBinding
{
	GENERATED_BODY()
	
	TSubclassOf<UGameplayAbility> AbilityClass;

	UPROPERTY()
	UInputAction* InputAction;

	EInputActionActivationType ActivationType;

	FAbilityBinding();

	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);
};

template<>
struct TStructOpsTypeTraits<FAbilityBinding> : public TStructOpsTypeTraitsBase2<FAbilityBinding>
{
	enum
	{
		WithNetSerializer = true,
	};
};

/*
 * Functions to manage abilities.
 * All abilities and effects can only have one instance.
 * This implementation trusts that duplicate abilities and effects are removed immediately on that frame.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TIMEGAME_API UAbilityManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAbilityManagerComponent();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void GrantAndBindActiveAbility(const TSubclassOf<UGameplayAbility>& Ability, UInputAction* InputAction, EInputActionActivationType ActivationType);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void RemoveActiveAbility(const TSubclassOf<UGameplayAbility>& Ability);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void UpdateAbilityBindings(const TArray<FAbilityBinding>& InAbilityBindings);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetCooldown(const TSubclassOf<UGameplayAbility>& Ability) const;

	void OnInput(const UInputAction* InputAction, const EInputActionActivationType ActivationType);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void GrantPassiveAbility(const TSubclassOf<UGameplayAbility>& Ability);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void RemovePassiveAbility(const TSubclassOf<UGameplayAbility>& Ability);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void GrantInfiniteEffect(const TSubclassOf<UGameplayEffect>& Effect);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void RemoveInfiniteEffect(const TSubclassOf<UGameplayEffect>& Effect);
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TMap<TSubclassOf<UGameplayAbility>, FGameplayAbilitySpecHandle> ServerGrantedAbilities;

	UPROPERTY()
	TMap<TSubclassOf<UGameplayEffect>, FActiveGameplayEffectHandle> ServerGrantedEffects;

	UPROPERTY()
	TArray<FAbilityBinding> AbilityBindings;

	UPROPERTY()
	ATgPlayerCharacter* PlayerCharacter;

	UAbilitySystemComponent* GetAsc() const;

	bool bRejectNextAbilityRemoval = false;

	int32 RejectNextEffectRemovals = 0;
};
