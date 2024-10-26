// Copyright (C) Varian Daemon 2023. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "O_CoreRecipeData.h"
#include "RD_TimeToCraft.generated.h"

/**
 * After the player requests the craft,
 * how long should it take to finish?
 */
UCLASS(DisplayName = "Time to Craft")
class IFP_CRAFTING_API URD_TimeToCraft : public UO_CoreRecipeData
{
	GENERATED_BODY()

public:

	UPROPERTY(Category = "Settings", EditAnywhere, BlueprintReadOnly)
	float TimeToCraft = 1;
};
