// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeGame/Public/Gas/TgHealingExecCalc.h"

#include "TimeGame/Public/Gas/TgAsc.h"
#include "TimeGame/Public/Gas/TgCreatureAttributeSet.h"

// Declare the attributes to capture and define how we want to capture them from the Source and Target.
struct FTgHealingStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
	DECLARE_ATTRIBUTE_CAPTUREDEF(HealMultiplier);

	FTgHealingStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTgCreatureAttributeSet, Health, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTgCreatureAttributeSet, HealMultiplier, Target, false);
	}
};

static const FTgHealingStatics& DamageStatics()
{
	static FTgHealingStatics HStatics;
	return HStatics;
}

UTgHealingExecCalc::UTgHealingExecCalc()
{
	RelevantAttributesToCapture.Add(DamageStatics().HealthDef);
	RelevantAttributesToCapture.Add(DamageStatics().HealMultiplierDef);
}

void UTgHealingExecCalc::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	float Healing = FMath::Max<float>(
		Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Healing")), false, -1.0f), 0.0f);

	const float OriginalHealing = Healing;

	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();

	// Gather the tags from the source and target as that can affect which buffs should be used
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float HealMultiplier = 0;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().HealMultiplierDef,
															   EvaluationParameters, HealMultiplier);
	HealMultiplier = FMath::Max<float>(HealMultiplier, 0);

	Healing *= HealMultiplier;

	//Output healing
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().HealthProperty, EGameplayModOp::Additive, Healing));

	//Broadcast healing
	if (UTgAsc* TargetAsc = Cast<UTgAsc>(TargetAbilitySystemComponent))
	{
		UTgAsc* SourceAsc = Cast<UTgAsc>(SourceAbilitySystemComponent);
		TargetAsc->BroadcastReceiveHealing(SourceAsc, Healing, OriginalHealing);
	}
}
