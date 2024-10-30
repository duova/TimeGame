// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/ZoneBase.h"

#include "AbilitySystemInterface.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

FZoneParams::FZoneParams()
{
}

AZoneBase::AZoneBase()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SetRootComponent(CollisionComponent);
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	MeshComponent->SetupAttachment(CollisionComponent);

	CollisionComponent->SetGenerateOverlapEvents(true);
	CollisionComponent->SetNotifyRigidBodyCollision(false);

	MeshComponent->SetGenerateOverlapEvents(false);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
}

AZoneBase* AZoneBase::SpawnZone(UObject* WorldContextObject, const FVector& Location, const FZoneParams& Params)
{
	AZoneBase* Zone = WorldContextObject->GetWorld()->SpawnActorDeferred<AZoneBase>(Params.ZoneClass, FTransform(FRotator::ZeroRotator, Location));

	Zone->OverlapEffectSpecs = Params.EffectSpecsOnOverlap;
	Zone->ActorsToIgnore = Params.InActorsToIgnore;

	if (Params.OverrideTickInterval >= 0)
	{
		Zone->TickInterval = Params.OverrideTickInterval;
	}

	if (Params.OverrideTickCountBeforeDespawn >= 0)
	{
		Zone->TickCountBeforeDespawn = Params.OverrideTickCountBeforeDespawn;
	}

	UGameplayStatics::FinishSpawningActor(Zone, FTransform(FRotator::ZeroRotator, Location));

	return Zone;
}

void AZoneBase::BeginPlay()
{
	Super::BeginPlay();
	if (!HasAuthority()) return;
	CollisionComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &AZoneBase::OnEnterOverlap);
	CollisionComponent->OnComponentEndOverlap.AddUniqueDynamic(this, &AZoneBase::OnEndOverlap);
	PrimaryActorTick.UpdateTickIntervalAndCoolDown(TickInterval);
}

void AZoneBase::OnEnterOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority()) return;
	if (ActorsToIgnore.Contains(Other)) return;
	if (const IAbilitySystemInterface* TargetAscInterface = Cast<IAbilitySystemInterface>(Other))
	{
		if (UAbilitySystemComponent* Asc = TargetAscInterface->GetAbilitySystemComponent())
		{
			if (!AscInZone.Contains(Asc)) AscInZone.Add(Asc);
		}
	}
}

void AZoneBase::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (!HasAuthority()) return;
	if (const IAbilitySystemInterface* TargetAscInterface = Cast<IAbilitySystemInterface>(Other))
	{
		if (const UAbilitySystemComponent* Asc = TargetAscInterface->GetAbilitySystemComponent())
		{
			if (AscInZone.Contains(Asc)) AscInZone.Remove(Asc);
		}
	}
}

void AZoneBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!HasAuthority()) return;

	for (UAbilitySystemComponent* Asc : AscInZone)
	{
		for (FGameplayEffectSpecHandle Spec : OverlapEffectSpecs)
		{
			Asc->BP_ApplyGameplayEffectSpecToTarget(Spec, Asc);
		}
	}
	
	TickCountBeforeDespawn--;
	if (TickCountBeforeDespawn <= 0) Destroy(true);
}

