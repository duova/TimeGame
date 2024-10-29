// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/TgGameState.h"

#include "Core/TgPlayerCharacter.h"

ATgGameState::ATgGameState()
{
}

void ATgGameState::SetWorldStateValue(const FGameplayTag Tag, const int32 Value)
{
	WorldStates.Contains(Tag) ? WorldStates[Tag] = Value : WorldStates.Add(Tag, Value);
}

void ATgGameState::ReadWorldStateValue(const FGameplayTag Tag, bool& Exists, int32& Value)
{
	if (!WorldStates.Contains(Tag))
	{
		Exists = false;
		return;
	}
	Exists = true;
	Value = WorldStates[Tag];
}

bool ATgGameState::WorldStateContains(const FGameplayTag Tag) const
{
	return WorldStates.Contains(Tag);
}

const ATgAiCharacter* ATgGameState::GetAiCharacterByNameTag(const FGameplayTag NameTag) const
{
	if (AiCharacterRegistry.Contains(NameTag))
	{
		return AiCharacterRegistry[NameTag];
	}
	return nullptr;
}

void ATgGameState::RegisterAiCharacter(const FGameplayTag& NameTag, ATgAiCharacter* Character)
{
	if (!HasAuthority()) return;
	if (AiCharacterRegistry.Contains(NameTag))
	{
		AiCharacterRegistry[NameTag] = Character;
	}
	else
	{
		AiCharacterRegistry.Add(NameTag, Character);
	}
}

void ATgGameState::BeginPlay()
{
	Super::BeginPlay();

	for (const auto& WorldState : InitialWorldStates)
	{
		if (WorldStates.Contains(WorldState.Tag))
		{
			UE_LOG(LogTimeGame, Warning, TEXT("Duplicate world state %s found. Removing latter."), *WorldState.Tag.ToString());
			continue;
		}
		WorldStates.Add(WorldState.Tag, WorldState.Value);
	}
}
