// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "Components/ArrowComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ProjectileBase.generated.h"

UCLASS()
class TIMEGAME_API AProjectileBase : public AActor
{
	GENERATED_BODY()

public:
	AProjectileBase();
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, meta = (WorldContext = "WorldContextObject"))
	static AProjectileBase* SpawnProjectile(UObject* WorldContextObject, const TSubclassOf<AProjectileBase> ProjectileClass,
	                                        FGameplayEffectSpec& EffectSpecOnOverlap,
	                                        FGameplayEffectSpec& EffectSpecOnHit, const FVector& Origin,
	                                        const FVector& Direction, const float Velocity,
	                                        const TArray<AActor*>& ActorsToIgnore = TArray<AActor*>());
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USphereComponent* CollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UArrowComponent* ArrowComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DespawnTime = 10.0;

public:
	virtual void Tick(float DeltaTime) override;
};
