// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeGame/Public/Core/TgPlayerState.h"

#include "AbilitySystemComponent.h"
#include "TimeGame/Public/Gas/TgCreatureAttributeSet.h"
#include "TimeGame/Public/Gas/TgAsc.h"

ATgPlayerState::ATgPlayerState()
{
	// Create ability system component, and set it to be explicitly replicated
	Asc = CreateDefaultSubobject<UTgAsc>(TEXT("AbilitySystemComponent"));
	Asc->SetIsReplicated(true);
	Asc->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	AttributeSet = CreateDefaultSubobject<UTgCreatureAttributeSet>(TEXT("AttributeSet"));

	AttributeSet->InitHealth(100);
	AttributeSet->InitEnergy(100);
	AttributeSet->InitMaxHealth(100);
	AttributeSet->InitMaxEnergy(100);
	AttributeSet->InitHealthRegen(1);
	AttributeSet->InitEnergyRegen(1);
	AttributeSet->InitShield(0);
	AttributeSet->InitIngenuity(100);
	AttributeSet->InitIntelligence(100);
	AttributeSet->InitArcane(100);
	AttributeSet->InitTelekinesis(100);
	AttributeSet->InitCooldownReduction(0);
	AttributeSet->InitPhysicalArmor(0);
	AttributeSet->InitFireArmor(0);
	AttributeSet->InitFrostArmor(0);
	AttributeSet->InitElectricalArmor(0);
	AttributeSet->InitMagicalArmor(0);
	AttributeSet->InitAlchemicalArmor(0);
	AttributeSet->InitLifesteal(0);
	AttributeSet->InitAntiHeal(0);
	AttributeSet->InitMovementSpeed(4000);
	AttributeSet->InitDamageIncomingMultiplier(1);
	AttributeSet->InitDamageOutgoingMultiplier(1);
}

UAbilitySystemComponent* ATgPlayerState::GetAbilitySystemComponent() const
{
	return Asc;
}
