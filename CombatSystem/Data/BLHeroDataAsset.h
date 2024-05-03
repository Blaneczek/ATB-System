// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BLCombatUtilities.h"
#include "BLHeroDataAsset.generated.h"


/**
 * 
 */
UCLASS(Blueprintable)
class BLADEOFLEGEND_API UBLHeroDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	// To auto calculate CurrentHP, test it more in future
	//virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	
	/** Takes data from the hero (stats) and calculates the variables needed for combat */
	FCombatCharData CalculateBaseCombatData(int32 Index);

	/** Adds to data asset reward from won combat */
	void AddCombatReward(int32 InExperience, int32 InMoney, const TArray<TSoftClassPtr<UBLItem>>& InItems);

	UFUNCTION(BlueprintCallable)
	float GetMaxHP(int32 Index) const;

	UFUNCTION(BlueprintCallable)
	float GetMaxME(int32 Index) const;

	void LevelUP();

	const TArray<FLevelUPData>& GetLevelUPData() const { return LevelUPData; }

	bool IsLevelUP() const { return bLevelUP; }

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
