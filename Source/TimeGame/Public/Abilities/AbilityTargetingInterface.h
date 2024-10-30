// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AbilityTargetingInterface.generated.h"

USTRUCT(BlueprintType)
struct FTargetingData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Origin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Target;
};

/**
 * Ability users pass targeting information through this interface to abilities.
 */
UINTERFACE(Blueprintable)
class UAbilityTargetingInterface : public UInterface
{
	GENERATED_BODY()
};

class IAbilityTargetingInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	FTargetingData GetTargetingData();
};