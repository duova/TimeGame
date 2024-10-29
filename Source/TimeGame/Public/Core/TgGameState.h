// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/GameState.h"
#include "TgGameState.generated.h"

class ATgAiCharacter;

USTRUCT(BlueprintType)
struct FTagIntPair
{
	GENERATED_BODY()

	FGameplayTag Tag = FGameplayTag();

	int32 Value = 0;
};

UCLASS()
class TIMEGAME_API ATgGameState : public AGameState
{
	GENERATED_BODY()

public:
	ATgGameState();
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void SetWorldStateValue(const FGameplayTag Tag, const int32 Value);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void ReadWorldStateValue(const FGameplayTag Tag, bool& Exists, int32& Value);

	UFUNCTION(BlueprintPure, BlueprintAuthorityOnly)
	bool WorldStateContains(const FGameplayTag Tag) const;

	UPROPERTY(EditAnywhere)
	TArray<FTagIntPair> InitialWorldStates;

	UFUNCTION(BlueprintPure, BlueprintAuthorityOnly)
	const ATgAiCharacter* GetAiCharacterByNameTag(const FGameplayTag NameTag) const;

	void RegisterAiCharacter(const FGameplayTag& NameTag, ATgAiCharacter* Character);

protected:
	UPROPERTY()
	TMap<FGameplayTag, int32> WorldStates;

	UPROPERTY()
	TMap<FGameplayTag, ATgAiCharacter*> AiCharacterRegistry;

	virtual void BeginPlay() override;
};
