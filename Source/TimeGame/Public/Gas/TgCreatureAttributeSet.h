// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "TgCreatureAttributeSet.generated.h"

// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * Attribute set used for all creatures that can receive damage and perform stat scaled actions. 
 */
UCLASS()
class TIMEGAME_API UTgCreatureAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	
	//Health
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UTgCreatureAttributeSet, Health)

	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);
	
	//MaxHealth
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UTgCreatureAttributeSet, MaxHealth)

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	//HealthRegen
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegen)
	FGameplayAttributeData HealthRegen;
	ATTRIBUTE_ACCESSORS(UTgCreatureAttributeSet, HealthRegen)

	UFUNCTION()
	virtual void OnRep_HealthRegen(const FGameplayAttributeData& OldHealthRegen);

	//Energy
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Energy)
	FGameplayAttributeData Energy;
	ATTRIBUTE_ACCESSORS(UTgCreatureAttributeSet, Energy)

	UFUNCTION()
	virtual void OnRep_Energy(const FGameplayAttributeData& OldEnergy);
	
	//MaxEnergy
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxEnergy)
	FGameplayAttributeData MaxEnergy;
	ATTRIBUTE_ACCESSORS(UTgCreatureAttributeSet, MaxEnergy)

	UFUNCTION()
	virtual void OnRep_MaxEnergy(const FGameplayAttributeData& OldMaxEnergy);

	//EnergyRegen
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_EnergyRegen)
	FGameplayAttributeData EnergyRegen;
	ATTRIBUTE_ACCESSORS(UTgCreatureAttributeSet, EnergyRegen)

	UFUNCTION()
	virtual void OnRep_EnergyRegen(const FGameplayAttributeData& OldEnergyRegen);

	//Shield
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Shield)
	FGameplayAttributeData Shield;
	ATTRIBUTE_ACCESSORS(UTgCreatureAttributeSet, Shield)

	UFUNCTION()
	virtual void OnRep_Shield(const FGameplayAttributeData& OldShield);
	
	//Ingenuity
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Ingenuity)
	FGameplayAttributeData Ingenuity;
	ATTRIBUTE_ACCESSORS(UTgCreatureAttributeSet, Ingenuity)

	UFUNCTION()
	virtual void OnRep_Ingenuity(const FGameplayAttributeData& OldIngenuity);

	//Intelligence
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Intelligence)
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UTgCreatureAttributeSet, Intelligence)

	UFUNCTION()
	virtual void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence);

	//Arcane
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Arcane)
	FGameplayAttributeData Arcane;
	ATTRIBUTE_ACCESSORS(UTgCreatureAttributeSet, Arcane)

	UFUNCTION()
	virtual void OnRep_Arcane(const FGameplayAttributeData& OldArcane);

	//Telekinesis
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Telekinesis)
	FGameplayAttributeData Telekinesis;
	ATTRIBUTE_ACCESSORS(UTgCreatureAttributeSet, Telekinesis)

	UFUNCTION()
	virtual void OnRep_Telekinesis(const FGameplayAttributeData& OldTelekinesis);

	//CooldownReduction
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CooldownReduction)
	FGameplayAttributeData CooldownReduction;
	ATTRIBUTE_ACCESSORS(UTgCreatureAttributeSet, CooldownReduction)

	UFUNCTION()
	virtual void OnRep_CooldownReduction(const FGameplayAttributeData& OldCooldownReduction);

	//PhysicalArmor
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_PhysicalArmor)
	FGameplayAttributeData PhysicalArmor;
	ATTRIBUTE_ACCESSORS(UTgCreatureAttributeSet, PhysicalArmor)

	UFUNCTION()
	virtual void OnRep_PhysicalArmor(const FGameplayAttributeData& OldPhysicalArmor);

	//FireArmor
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_FireArmor)
	FGameplayAttributeData FireArmor;
	ATTRIBUTE_ACCESSORS(UTgCreatureAttributeSet, FireArmor)

	UFUNCTION()
	virtual void OnRep_FireArmor(const FGameplayAttributeData& OldFireArmor);

	//FrostArmor
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_FrostArmor)
	FGameplayAttributeData FrostArmor;
	ATTRIBUTE_ACCESSORS(UTgCreatureAttributeSet, FrostArmor)

	UFUNCTION()
	virtual void OnRep_FrostArmor(const FGameplayAttributeData& OldFrostArmor);

	//ElectricalArmor
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ElectricalArmor)
	FGameplayAttributeData ElectricalArmor;
	ATTRIBUTE_ACCESSORS(UTgCreatureAttributeSet, ElectricalArmor)

	UFUNCTION()
	virtual void OnRep_ElectricalArmor(const FGameplayAttributeData& OldElectricalArmor);

	//AlchemicalArmor
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_AlchemicalArmor)
	FGameplayAttributeData AlchemicalArmor;
	ATTRIBUTE_ACCESSORS(UTgCreatureAttributeSet, AlchemicalArmor)

	UFUNCTION()
	virtual void OnRep_AlchemicalArmor(const FGameplayAttributeData& OldAlchemicalArmor);

	//MagicalArmor
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MagicalArmor)
	FGameplayAttributeData MagicalArmor;
	ATTRIBUTE_ACCESSORS(UTgCreatureAttributeSet, MagicalArmor)

	UFUNCTION()
	virtual void OnRep_MagicalArmor(const FGameplayAttributeData& OldMagicalArmor);

	//Lifesteal
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Lifesteal)
	FGameplayAttributeData Lifesteal;
	ATTRIBUTE_ACCESSORS(UTgCreatureAttributeSet, Lifesteal)

	UFUNCTION()
	virtual void OnRep_Lifesteal(const FGameplayAttributeData& OldLifesteal);

	//MovementSpeed
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MovementSpeed)
	FGameplayAttributeData MovementSpeed;
	ATTRIBUTE_ACCESSORS(UTgCreatureAttributeSet, MovementSpeed)

	UFUNCTION()
	virtual void OnRep_MovementSpeed(const FGameplayAttributeData& OldMovementSpeed);

	//HealMultiplier
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealMultiplier)
	FGameplayAttributeData HealMultiplier;
	ATTRIBUTE_ACCESSORS(UTgCreatureAttributeSet, HealMultiplier)

	UFUNCTION()
	virtual void OnRep_HealMultiplier(const FGameplayAttributeData& OldHealMultiplier);

	//DamageOutgoingMultiplier
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_DamageOutgoingMultiplier)
	FGameplayAttributeData DamageOutgoingMultiplier;
	ATTRIBUTE_ACCESSORS(UTgCreatureAttributeSet, DamageOutgoingMultiplier)

	UFUNCTION()
	virtual void OnRep_DamageOutgoingMultiplier(const FGameplayAttributeData& OldDamageOutgoingMultiplier);

	//DamageIncomingMultiplier
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_DamageIncomingMultiplier)
	FGameplayAttributeData DamageIncomingMultiplier;
	ATTRIBUTE_ACCESSORS(UTgCreatureAttributeSet, DamageIncomingMultiplier)

	UFUNCTION()
	virtual void OnRep_DamageIncomingMultiplier(const FGameplayAttributeData& OldDamageIncomingMultiplier);
};
