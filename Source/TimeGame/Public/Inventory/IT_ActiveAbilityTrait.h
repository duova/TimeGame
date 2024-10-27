// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Core/Traits/ItemTrait.h"

#include "IT_ActiveAbilityTrait.generated.h"

/**
 * Trait that tracks the abilities slotting this item binds to.
 */
UCLASS()
class TIMEGAME_API UIT_ActiveAbilityTrait : public UItemTrait
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UGameplayAbility> InputDownAbility;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UGameplayAbility> InputUpAbility;
};
