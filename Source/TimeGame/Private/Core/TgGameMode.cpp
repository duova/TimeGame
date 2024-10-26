// Copyright Epic Games, Inc. All Rights Reserved.

#include "TimeGame/Public/Core/TgGameMode.h"
#include "UObject/ConstructorHelpers.h"

ATgGameMode::ATgGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
