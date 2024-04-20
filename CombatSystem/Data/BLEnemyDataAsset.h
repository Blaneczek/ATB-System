// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BLCombatUtilities.h"
#include "BLEnemyDataAsset.generated.h"

class UBLAction;

USTRUCT()
struct FEnemyAssetInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FCombatCharData BaseData;

	UPROPERTY(EditAnywhere)
	TArray<TSoftClassPtr<UBLAction>> AttackActions;

	UPROPERTY(EditAnywhere)
	TArray<TSoftClassPtr<UBLAction>> DefendActions;

	UPROPERTY(EditAnywhere)
	TArray<TSoftClassPtr<UBLAction>> SpecialActions;
};

/**
 * 
 */
UCLASS()
class BLADEOFLEGEND_API UBLEnemyDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	TArray<FEnemyAssetInfo> Enemies;
};
