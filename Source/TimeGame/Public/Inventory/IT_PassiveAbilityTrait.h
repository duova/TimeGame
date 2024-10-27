// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Core/Traits/ItemTrait.h"
#include "IT_PassiveAbilityTrait.generated.h"

/**
 * Trait that tracks the abilities and effects granted by this item.
 */
UCLASS()
class TIMEGAME_API UIT_PassiveAbilityTrait : public UItemTrait
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UGameplayAbility> PassiveAbility;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<TSubclassOf<UGameplayEffect>> Effects;
};
