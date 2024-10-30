// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "ZoneBase.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ProjectileBase.generated.h"

UCLASS()
class TIMEGAME_API AProjectileBase : public AActor
{
	GENERATED_BODY()

public:
	AProjectileBase();
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, meta = (WorldContext = "WorldContextObject", AutoCreateRefTerm = "EffectSpecsOnOverlap, ProjectileClass, InActorsToIgnore, InZoneParams"))
	static AProjectileBase* SpawnProjectile(UObject* WorldContextObject, const TSubclassOf<AProjectileBase>& ProjectileClass,
	                                        const TArray<FGameplayEffectSpecHandle>& EffectSpecsOnOverlap, const FVector& Origin,
	                                        const FVector& Direction, const bool bInDestroyOnOverlap, const TArray<AActor*>& InActorsToIgnore, const bool bInCreateZone, const FZoneParams& InZoneParams);
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UPrimitiveComponent* CollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UArrowComponent* ArrowComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MeshComponent;

	TArray<FGameplayEffectSpecHandle> OverlapEffectSpecs;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DespawnTime = 5.0;

	UPROPERTY()
	TArray<AActor*> ActorsToIgnore;

	bool bDestroyOnOverlap;

	bool bCreateZone;

	FZoneParams ZoneParams;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	virtual void Tick(float DeltaTime) override;
};
