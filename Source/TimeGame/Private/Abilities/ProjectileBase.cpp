// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/ProjectileBase.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Kismet/GameplayStatics.h"


AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicatingMovement(true);
	
	CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SetRootComponent(CollisionComponent);
	ArrowComponent = CreateDefaultSubobject<UArrowComponent>("ArrowComponent");
	ArrowComponent->SetupAttachment(CollisionComponent);
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	MeshComponent->SetupAttachment(CollisionComponent);
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	ProjectileMovementComponent->SetIsReplicated(true);

	CollisionComponent->SetGenerateOverlapEvents(true);
	CollisionComponent->SetNotifyRigidBodyCollision(true);

	ProjectileMovementComponent->bRotationFollowsVelocity;
	ProjectileMovementComponent->bInterpMovement = true;
	ProjectileMovementComponent->SetInterpolatedComponent(MeshComponent);
	ProjectileMovementComponent->ProjectileGravityScale = 0;
	ProjectileMovementComponent->MaxSpeed = 4000;
	ProjectileMovementComponent->InitialSpeed = 4000;

	MeshComponent->SetGenerateOverlapEvents(false);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
}

AProjectileBase* AProjectileBase::SpawnProjectile(UObject* WorldContextObject,
	const TSubclassOf<AProjectileBase>& ProjectileClass, const FGameplayEffectSpecHandle& EffectSpecOnOverlap, const FVector& Origin, const FVector& Direction, const bool bInDestroyOnOverlap, const TArray<AActor*>& InActorsToIgnore)
{
	const FRotator Rot = Direction.Rotation();
	AProjectileBase* Projectile = WorldContextObject->GetWorld()->SpawnActorDeferred<AProjectileBase>(ProjectileClass, FTransform(Rot, Origin));

	Projectile->OverlapEffectSpec = EffectSpecOnOverlap;

	Projectile->ActorsToIgnore = InActorsToIgnore;
	Projectile->bDestroyOnOverlap = bInDestroyOnOverlap;

	UGameplayStatics::FinishSpawningActor(Projectile, FTransform(Rot, Origin));

	return Projectile;
}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority()) return;
	CollisionComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &AProjectileBase::OnOverlap);
}

void AProjectileBase::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority()) return;
	if (ActorsToIgnore.Contains(Other)) return;
	if (const IAbilitySystemInterface* TargetAscInterface = Cast<IAbilitySystemInterface>(Other))
	{
		if (UAbilitySystemComponent* Asc = TargetAscInterface->GetAbilitySystemComponent())
		{
			if (!OverlapEffectSpec.Data) return;
			
			OverlapEffectSpec.Data->GetContext().AddHitResult(SweepResult);
			Asc->BP_ApplyGameplayEffectSpecToTarget(OverlapEffectSpec, Asc);
		}
	}
	if (bDestroyOnOverlap) Destroy(true);
}

void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!HasAuthority()) return;

	DespawnTime -= DeltaTime;
	if (DespawnTime <= 0) Destroy(true);
}

