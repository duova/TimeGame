// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "TgAsc.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnReceiveDamage, UAbilitySystemComponent*, Source, float, ProcessedShieldDamage, float, ProcessedHealthDamage, float, OriginalDamage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnReceiveHealing, UAbilitySystemComponent*, Source, float, ProcessedHealing, float, OriginalHealing);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHit, UAbilitySystemComponent*, Source, UAbilitySystemComponent*, Target);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TIMEGAME_API UTgAsc : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UTgAsc();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable)
	FOnReceiveDamage OnReceiveDamage;

	UPROPERTY(BlueprintAssignable)
	FOnReceiveHealing OnReceiveHealing;

	UPROPERTY(BlueprintAssignable)
	FOnHit OnHitRegistry;

	void BroadcastReceiveDamage(UAbilitySystemComponent* Source, const float ProcessedShieldDamage, const float ProcessedHealthDamage, const float OriginalDamage);

	void BroadcastReceiveHealing(UAbilitySystemComponent* Source, const float ProcessedHealing, const float OriginalHealing);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void PerformOnHit(UAbilitySystemComponent* Target);
};
