// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeGame/Public/Gas/TgDamageExecCalc.h"

#include "TimeGame/Public/Gas/TgAsc.h"
#include "TimeGame/Public/Gas/TgCreatureAttributeSet.h"
#include "TimeGame/Public/Gas/TgHealingExecCalc.h"

// Declare the attributes to capture and define how we want to capture them from the Source and Target.
struct FTgDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
	DECLARE_ATTRIBUTE_CAPTUREDEF(HealMultiplier);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Shield);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalArmor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireArmor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(FrostArmor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ElectricalArmor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(AlchemicalArmor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MagicalArmor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Lifesteal);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageOutgoingMultiplier);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageIncomingMultiplier);

	FTgDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTgCreatureAttributeSet, Health, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTgCreatureAttributeSet, HealMultiplier, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTgCreatureAttributeSet, Shield, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTgCreatureAttributeSet, PhysicalArmor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTgCreatureAttributeSet, FireArmor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTgCreatureAttributeSet, FrostArmor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTgCreatureAttributeSet, ElectricalArmor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTgCreatureAttributeSet, AlchemicalArmor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTgCreatureAttributeSet, MagicalArmor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTgCreatureAttributeSet, Lifesteal, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTgCreatureAttributeSet, DamageOutgoingMultiplier, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTgCreatureAttributeSet, DamageIncomingMultiplier, Target, false);
	}
};

static const FTgDamageStatics& DamageStatics()
{
	static FTgDamageStatics DStatics;
	return DStatics;
}

UTgDamageExecCalc::UTgDamageExecCalc()
{
	RelevantAttributesToCapture.Add(DamageStatics().ShieldDef);
	RelevantAttributesToCapture.Add(DamageStatics().PhysicalArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().FireArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().FrostArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().ElectricalArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().AlchemicalArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().MagicalArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().LifestealDef);
	RelevantAttributesToCapture.Add(DamageStatics().DamageOutgoingMultiplierDef);
	RelevantAttributesToCapture.Add(DamageStatics().DamageIncomingMultiplierDef);
	RelevantAttributesToCapture.Add(DamageStatics().HealthDef);
	RelevantAttributesToCapture.Add(DamageStatics().HealMultiplierDef);
}

void UTgDamageExecCalc::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                               FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	float Damage = FMath::Max<float>(
		Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), false, -1.0f), 0.0f);

	float OriginalDamage = Damage;

	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();

	// Gather the tags from the source and target as that can affect which buffs should be used
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	FGameplayTagContainer SpecAssetTags;
	Spec.GetAllAssetTags(SpecAssetTags);

	float DamageOutgoingMultiplier = 0;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DamageOutgoingMultiplierDef,
	                                                           EvaluationParameters, DamageOutgoingMultiplier);
	DamageOutgoingMultiplier = FMath::Max<float>(DamageOutgoingMultiplier, 0);

	float DamageIncomingMultiplier = 0;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DamageIncomingMultiplierDef,
	                                                           EvaluationParameters, DamageIncomingMultiplier);
	DamageIncomingMultiplier = FMath::Max<float>(DamageIncomingMultiplier, 0);

	Damage *= DamageOutgoingMultiplier * DamageIncomingMultiplier;

	OriginalDamage *= DamageOutgoingMultiplier;

	if (SpecAssetTags.HasTag(FGameplayTag::RequestGameplayTag(FName("Effect.Damage.Physical"))))
	{
		float PhysicalArmor = 0;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().PhysicalArmorDef,
		                                                           EvaluationParameters, PhysicalArmor);
		PhysicalArmor = FMath::Clamp(PhysicalArmor, 0, 0.8);
		Damage *= 1.0 - PhysicalArmor;
	}

	if (SpecAssetTags.HasTag(FGameplayTag::RequestGameplayTag(FName("Effect.Damage.Fire"))))
	{
		float FireArmor = 0;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().FireArmorDef, EvaluationParameters,
		                                                           FireArmor);
		FireArmor = FMath::Clamp(FireArmor, 0, 0.8);
		Damage *= 1.0 - FireArmor;
	}

	if (SpecAssetTags.HasTag(FGameplayTag::RequestGameplayTag(FName("Effect.Damage.Frost"))))
	{
		float FrostArmor = 0;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().FrostArmorDef, EvaluationParameters,
		                                                           FrostArmor);
		FrostArmor = FMath::Clamp(FrostArmor, 0, 0.8);
		Damage *= 1.0 - FrostArmor;
	}

	if (SpecAssetTags.HasTag(FGameplayTag::RequestGameplayTag(FName("Effect.Damage.Electrical"))))
	{
		float ElectricalArmor = 0;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ElectricalArmorDef,
		                                                           EvaluationParameters, ElectricalArmor);
		ElectricalArmor = FMath::Clamp(ElectricalArmor, 0, 0.8);
		Damage *= 1.0 - ElectricalArmor;
	}

	if (SpecAssetTags.HasTag(FGameplayTag::RequestGameplayTag(FName("Effect.Damage.Alchemical"))))
	{
		float AlchemicalArmor = 0;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().AlchemicalArmorDef,
		                                                           EvaluationParameters, AlchemicalArmor);
		AlchemicalArmor = FMath::Clamp(AlchemicalArmor, 0, 0.8);
		Damage *= 1.0 - AlchemicalArmor;
	}

	if (SpecAssetTags.HasTag(FGameplayTag::RequestGameplayTag(FName("Effect.Damage.Magical"))))
	{
		float MagicalArmor = 0;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().MagicalArmorDef,
		                                                           EvaluationParameters, MagicalArmor);
		MagicalArmor = FMath::Clamp(MagicalArmor, 0, 0.8);
		Damage *= 1.0 - MagicalArmor;
	}

	if (SpecAssetTags.HasTag(FGameplayTag::RequestGameplayTag(FName("Effect.Damage.Magical"))))
	{
		float MagicalArmor = 0;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().MagicalArmorDef,
		                                                           EvaluationParameters, MagicalArmor);
		MagicalArmor = FMath::Clamp(MagicalArmor, 0, 0.8);
		Damage *= 1.0 - MagicalArmor;
	}

	float ShieldedDamage = 0;
	float Shield = 0;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ShieldDef, EvaluationParameters,
	                                                           Shield);
	ShieldedDamage = FMath::Min<float>(Shield, Damage);
	Damage -= ShieldedDamage;
	
	if (SpecAssetTags.HasTag(FGameplayTag::RequestGameplayTag(FName("Effect.Damage.CanLifesteal"))))
	{
		float Lifesteal = 0;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().LifestealDef,
																   EvaluationParameters, Lifesteal);
		Lifesteal = FMath::Max<float>(Lifesteal, 0);
		const float StolenHealth = Lifesteal * Damage;
		
		float HealMultiplier = 0;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().HealMultiplierDef,
																   EvaluationParameters, HealMultiplier);
		HealMultiplier = FMath::Max<float>(0, HealMultiplier);
		const float ProcessedStolenHealth = StolenHealth * HealMultiplier;

		UGameplayEffect* GELifesteal = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("Lifesteal")));
		GELifesteal->DurationPolicy = EGameplayEffectDurationType::Instant;

		int32 Idx = GELifesteal->Modifiers.Num();
		GELifesteal->Modifiers.SetNum(Idx + 1);
		FGameplayModifierInfo& Info = GELifesteal->Modifiers[Idx];
		Info.ModifierMagnitude = FScalableFloat(ProcessedStolenHealth);
		Info.ModifierOp = EGameplayModOp::Additive;
		Info.Attribute = UTgCreatureAttributeSet::GetHealthAttribute();

		SourceAbilitySystemComponent->ApplyGameplayEffectToSelf(GELifesteal, 1.0f, SourceAbilitySystemComponent->MakeEffectContext());
		//Broadcast healing.
		if (UTgAsc* SourceAsc = Cast<UTgAsc>(SourceAbilitySystemComponent))
		{
			SourceAsc->BroadcastReceiveHealing(SourceAsc, ProcessedStolenHealth, StolenHealth);
		}
	}

	//Set output.
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().HealthProperty, EGameplayModOp::Additive, -Damage));
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().ShieldProperty, EGameplayModOp::Additive, -ShieldedDamage));

	//Broadcast damage.
	if (UTgAsc* TargetAsc = Cast<UTgAsc>(TargetAbilitySystemComponent))
	{
		UTgAsc* SourceAsc = Cast<UTgAsc>(SourceAbilitySystemComponent);
		TargetAsc->BroadcastReceiveDamage(SourceAsc, ShieldedDamage, Damage, OriginalDamage);
	}
}
