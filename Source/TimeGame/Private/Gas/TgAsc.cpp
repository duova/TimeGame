// Fill out your copyright notice in the Description page of Project Settings.

#include "TimeGame/Public/Gas/TgAsc.h"

UTgAsc::UTgAsc()
{
}

void UTgAsc::BeginPlay()
{
	Super::BeginPlay();
}

void UTgAsc::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UTgAsc::BroadcastReceiveDamage(UAbilitySystemComponent* Source, const float ProcessedShieldDamage,
	const float ProcessedHealthDamage, const float OriginalDamage)
{
	if (OnReceiveDamage.IsBound())
	{
		OnReceiveDamage.Broadcast(Source, ProcessedShieldDamage, ProcessedHealthDamage, OriginalDamage);
	}
}

void UTgAsc::BroadcastReceiveHealing(UAbilitySystemComponent* Source, const float ProcessedHealing,
	const float OriginalHealing)
{
	if (OnReceiveHealing.IsBound())
	{
		OnReceiveHealing.Broadcast(Source, ProcessedHealing, OriginalHealing);
	}
}

