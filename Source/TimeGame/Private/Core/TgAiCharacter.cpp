// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/TgAiCharacter.h"

#include "Core/TgGameState.h"

ATgAiCharacter::ATgAiCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATgAiCharacter::BeginPlay()
{
	Super::BeginPlay();

	ATgGameState* Gs = Cast<ATgGameState>(GetWorld()->GetGameState());
	Gs->RegisterAiCharacter(NameTag, this);
}

void ATgAiCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

