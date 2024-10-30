// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Actor.h"
#include "ZoneBase.generated.h"

class AZoneBase;

USTRUCT(BlueprintType)
struct FZoneParams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AZoneBase> ZoneClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FGameplayEffectSpecHandle> EffectSpecsOnOverlap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> InActorsToIgnore;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OverrideTickInterval = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int OverrideTickCountBeforeDespawn = -1;

	FZoneParams();
};

UCLASS()
class TIMEGAME_API AZoneBase : public AActor
{
	GENERATED_BODY()

public:
	AZoneBase();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly,
		meta = (WorldContext = "WorldContextObject"))
	static AZoneBase* SpawnZone(UObject* WorldContextObject, const FVector& Location, const FZoneParams& Params);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UPrimitiveComponent* CollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MeshComponent;

	TArray<FGameplayEffectSpecHandle> OverlapEffectSpecs;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = 0.0))
	float TickInterval = 0.5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = 0))
	int TickCountBeforeDespawn = 10;

	UPROPERTY()
	TSet<UAbilitySystemComponent*> AscInZone;
	
	UPROPERTY()
	TArray<AActor*> ActorsToIgnore;

	UFUNCTION()
	void OnEnterOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	virtual void Tick(float DeltaTime) override;
};
