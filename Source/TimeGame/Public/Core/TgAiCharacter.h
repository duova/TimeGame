// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Character.h"
#include "TgAiCharacter.generated.h"

UCLASS()
class TIMEGAME_API ATgAiCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ATgAiCharacter();

protected:
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;
	
	void OnMovementSpeedAttributeUpdated(const FOnAttributeChangeData& Data) const;

	UPROPERTY(EditAnywhere)
	FGameplayAttribute MovementSpeedAttribute;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag NameTag;

	UPROPERTY(EditAnywhere)
	UAbilitySystemComponent* Asc;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	virtual void Tick(float DeltaTime) override;
};
