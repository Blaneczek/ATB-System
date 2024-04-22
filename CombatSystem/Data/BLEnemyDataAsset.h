// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BLCombatUtilities.h"
#include "BLEnemyDataAsset.generated.h"

class UBLAction;

USTRUCT(BlueprintType)
struct FEnemyAssetInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCombatCharData BaseData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSoftClassPtr<UBLAction>> AttackActions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSoftClassPtr<UBLAction>> DefendActions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSoftClassPtr<UBLAction>> SpecialActions;
};

/**
 * 
 */
UCLASS(Blueprintable)
class BLADEOFLEGEND_API UBLEnemyDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable)
	void SetData(const TArray<FEnemyAssetInfo>& InEnemies);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FEnemyAssetInfo> Enemies;
};
