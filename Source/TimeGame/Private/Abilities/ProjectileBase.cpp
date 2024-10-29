// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/ProjectileBase.h"


AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicatingMovement(true);
	
	CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SetRootComponent(CollisionComponent);
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	ProjectileMovementComponent->SetIsReplicated(true);
	ArrowComponent = CreateDefaultSubobject<UArrowComponent>("ArrowComponent");
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");

	CollisionComponent->SetGenerateOverlapEvents(true);
	CollisionComponent->SetNotifyRigidBodyCollision(true);

	ProjectileMovementComponent->bRotationFollowsVelocity;
	ProjectileMovementComponent->bInterpMovement;
	ProjectileMovementComponent->SetInterpolatedComponent(MeshComponent);
}

AProjectileBase* AProjectileBase::SpawnProjectile(UObject* WorldContextObject,
	const TSubclassOf<AProjectileBase> ProjectileClass, FGameplayEffectSpec& EffectSpecOnOverlap,
	FGameplayEffectSpec& EffectSpecOnHit, const FVector& Origin, const FVector& Direction, const float Velocity,
	const TArray<AActor*>& ActorsToIgnore)
{
	AProjectileBase* Projectile = Cast<AProjectileBase>(WorldContextObject->GetWorld()->SpawnActor(ProjectileClass, &Origin));
}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!HasAuthority()) return;

	DespawnTime -= DeltaTime;
	if (DespawnTime <= 0) Destroy(true);
}

