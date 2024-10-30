// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/TgAiCharacter.h"

#include "Core/TgGameState.h"
#include "Core/TgPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Gas/TgAsc.h"

ATgAiCharacter::ATgAiCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	Asc = CreateDefaultSubobject<UTgAsc>(TEXT("AbilitySystemComponent"));
	Asc->SetIsReplicated(true);
}

void ATgAiCharacter::BeginPlay()
{
	Super::BeginPlay();

	ATgGameState* Gs = Cast<ATgGameState>(GetWorld()->GetGameState());
	Gs->RegisterAiCharacter(NameTag, this);
}

void ATgAiCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (Asc)
	{
		Asc->InitAbilityActorInfo(this, this);
	}
	
	SetOwner(NewController);

	if (MovementSpeedAttribute.IsValid())
	{
		Asc->GetGameplayAttributeValueChangeDelegate(MovementSpeedAttribute).AddUObject(this, &ATgAiCharacter::OnMovementSpeedAttributeUpdated);
	}
	else
	{
		UE_LOG(LogTimeGame, Error, TEXT("MovementSpeed attribute needs to be set on AiCharacter."));
	}
}

void ATgAiCharacter::OnMovementSpeedAttributeUpdated(const FOnAttributeChangeData& Data) const
{
	GetCharacterMovement()->MaxWalkSpeed = Data.NewValue;
}

UAbilitySystemComponent* ATgAiCharacter::GetAbilitySystemComponent() const
{
	return Asc;
}

void ATgAiCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

