// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Character.h"
#include "TgAiCharacter.generated.h"

UCLASS()
class TIMEGAME_API ATgAiCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATgAiCharacter();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag NameTag;
	
	virtual void Tick(float DeltaTime) override;
};
