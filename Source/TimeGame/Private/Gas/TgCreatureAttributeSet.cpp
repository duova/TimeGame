// Fill out your copyright notice in the Description page of Project Settings.

#include "TimeGame/Public/Gas/TgCreatureAttributeSet.h"

#include "Net/UnrealNetwork.h"

void UTgCreatureAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UTgCreatureAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTgCreatureAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTgCreatureAttributeSet, HealthRegen, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTgCreatureAttributeSet, Energy, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTgCreatureAttributeSet, MaxEnergy, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTgCreatureAttributeSet, EnergyRegen, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTgCreatureAttributeSet, Shield, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTgCreatureAttributeSet, Ingenuity, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTgCreatureAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTgCreatureAttributeSet, Arcane, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTgCreatureAttributeSet, Telekinesis, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTgCreatureAttributeSet, CooldownReduction, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTgCreatureAttributeSet, PhysicalArmor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTgCreatureAttributeSet, FireArmor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTgCreatureAttributeSet, FrostArmor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTgCreatureAttributeSet, ElectricalArmor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTgCreatureAttributeSet, AlchemicalArmor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTgCreatureAttributeSet, MagicalArmor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTgCreatureAttributeSet, Lifesteal, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTgCreatureAttributeSet, MovementSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTgCreatureAttributeSet, HealMultiplier, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTgCreatureAttributeSet, DamageOutgoingMultiplier, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTgCreatureAttributeSet, DamageIncomingMultiplier, COND_None, REPNOTIFY_Always);
}

void UTgCreatureAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0, GetMaxHealth());
	}

	if (Attribute == GetEnergyAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0, GetMaxEnergy());
	}

	if (Attribute == GetShieldAttribute())
	{
		NewValue = FMath::Max<float>(0, NewValue);
	}

	if (Attribute == GetCooldownReductionAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0, 0.8);
	}

	if (Attribute == GetPhysicalArmorAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0, 0.8);
	}

	if (Attribute == GetFireArmorAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0, 0.8);
	}

	if (Attribute == GetFrostArmorAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0, 0.8);
	}

	if (Attribute == GetElectricalArmorAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0, 0.8);
	}

	if (Attribute == GetAlchemicalArmorAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0, 0.8);
	}

	if (Attribute == GetMagicalArmorAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0, 0.8);
	}

	if (Attribute == GetMovementSpeedAttribute())
	{
		NewValue = FMath::Max<float>(0, NewValue);
	}

	if (Attribute == GetHealMultiplierAttribute())
	{
		NewValue = FMath::Max<float>(0, NewValue);
	}

	if (Attribute == GetDamageOutgoingMultiplierAttribute())
	{
		NewValue = FMath::Max<float>(0, NewValue);
	}

	if (Attribute == GetDamageIncomingMultiplierAttribute())
	{
		NewValue = FMath::Max<float>(0, NewValue);
	}
}

void UTgCreatureAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTgCreatureAttributeSet, Health, OldHealth);
}

void UTgCreatureAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTgCreatureAttributeSet, MaxHealth, OldMaxHealth);
}

void UTgCreatureAttributeSet::OnRep_HealthRegen(const FGameplayAttributeData& OldHealthRegen)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTgCreatureAttributeSet, HealthRegen, OldHealthRegen);
}

void UTgCreatureAttributeSet::OnRep_Energy(const FGameplayAttributeData& OldEnergy)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTgCreatureAttributeSet, Energy, OldEnergy);
}

void UTgCreatureAttributeSet::OnRep_MaxEnergy(const FGameplayAttributeData& OldMaxEnergy)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTgCreatureAttributeSet, MaxEnergy, OldMaxEnergy);
}

void UTgCreatureAttributeSet::OnRep_EnergyRegen(const FGameplayAttributeData& OldEnergyRegen)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTgCreatureAttributeSet, EnergyRegen, OldEnergyRegen);
}

void UTgCreatureAttributeSet::OnRep_Shield(const FGameplayAttributeData& OldShield)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTgCreatureAttributeSet, Shield, OldShield);
}

void UTgCreatureAttributeSet::OnRep_Ingenuity(const FGameplayAttributeData& OldIngenuity)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTgCreatureAttributeSet, Ingenuity, OldIngenuity);
}

void UTgCreatureAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTgCreatureAttributeSet, Intelligence, OldIntelligence);
}

void UTgCreatureAttributeSet::OnRep_Arcane(const FGameplayAttributeData& OldArcane)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTgCreatureAttributeSet, Arcane, OldArcane);
}

void UTgCreatureAttributeSet::OnRep_Telekinesis(const FGameplayAttributeData& OldTelekinesis)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTgCreatureAttributeSet, Telekinesis, OldTelekinesis);
}

void UTgCreatureAttributeSet::OnRep_CooldownReduction(const FGameplayAttributeData& OldCooldownReduction)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTgCreatureAttributeSet, CooldownReduction, OldCooldownReduction);
}

void UTgCreatureAttributeSet::OnRep_PhysicalArmor(const FGameplayAttributeData& OldPhysicalArmor)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTgCreatureAttributeSet, PhysicalArmor, OldPhysicalArmor);
}

void UTgCreatureAttributeSet::OnRep_FireArmor(const FGameplayAttributeData& OldFireArmor)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTgCreatureAttributeSet, FireArmor, OldFireArmor);
}

void UTgCreatureAttributeSet::OnRep_FrostArmor(const FGameplayAttributeData& OldFrostArmor)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTgCreatureAttributeSet, FrostArmor, OldFrostArmor);
}

void UTgCreatureAttributeSet::OnRep_ElectricalArmor(const FGameplayAttributeData& OldElectricalArmor)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTgCreatureAttributeSet, ElectricalArmor, OldElectricalArmor);
}

void UTgCreatureAttributeSet::OnRep_AlchemicalArmor(const FGameplayAttributeData& OldAlchemicalArmor)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTgCreatureAttributeSet, AlchemicalArmor, OldAlchemicalArmor);
}

void UTgCreatureAttributeSet::OnRep_MagicalArmor(const FGameplayAttributeData& OldMagicalArmor)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTgCreatureAttributeSet, MagicalArmor, OldMagicalArmor);
}

void UTgCreatureAttributeSet::OnRep_Lifesteal(const FGameplayAttributeData& OldLifesteal)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTgCreatureAttributeSet, Lifesteal, OldLifesteal);
}

void UTgCreatureAttributeSet::OnRep_MovementSpeed(const FGameplayAttributeData& OldMovementSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTgCreatureAttributeSet, MovementSpeed, OldMovementSpeed);
}

void UTgCreatureAttributeSet::OnRep_HealMultiplier(const FGameplayAttributeData& OldHealMultiplier)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTgCreatureAttributeSet, HealMultiplier, OldHealMultiplier);
}

void UTgCreatureAttributeSet::OnRep_DamageOutgoingMultiplier(const FGameplayAttributeData& OldDamageOutgoingMultiplier)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTgCreatureAttributeSet, DamageOutgoingMultiplier, OldDamageOutgoingMultiplier);
}

void UTgCreatureAttributeSet::OnRep_DamageIncomingMultiplier(const FGameplayAttributeData& OldDamageIncomingMultiplier)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTgCreatureAttributeSet, DamageIncomingMultiplier, OldDamageIncomingMultiplier);
}
