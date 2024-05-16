// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Core/BLCombatUtilities.h"
#include "BLHeroDataAsset.generated.h"


/**
 * 
 */
UCLASS(Blueprintable)
class BLADEOFLEGEND_API UBLHeroDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	FHeroDataAssetData GetDataAssetAsStruct();

	UFUNCTION(BlueprintCallable)
	void SetDataAssetFromStruct(const FHeroDataAssetData& Data);

public:
	/** Player's heroes */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FHeroAssetInfo> Heroes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Experience;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ExperienceNextLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSoftClassPtr<UBLItem>> Items;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Money;

private:
	UPROPERTY()
	TArray<FLevelUPData> LevelUPData;

	UPROPERTY()
	bool bLevelUP = false;
};
