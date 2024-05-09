// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actions/BLAction.h"
#include "BLSummonAction.generated.h"


USTRUCT(BlueprintType)
struct FEnemiesSpawnData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BL|Combat")
	int32 Level;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BL|Combat")
	FCombatCharData BaseData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BL|Combat")
	TArray<TSoftClassPtr<UBLAction>> Actions;

	FEnemiesSpawnData()
		: Level(1)
	{}

	FEnemiesSpawnData(int32 InLevel, const FCombatCharData& InBaseData, const TArray<TSoftClassPtr<UBLAction>>& InActions)
		: Level(InLevel), BaseData(InBaseData), Actions(InActions)
	{}
};

/**
 * 
 */
UCLASS(Blueprintable)
class BLADEOFLEGEND_API UBLSummonAction : public UBLAction
{
	GENERATED_BODY()

public:
	void ActivateAction(UBLActionComponent* OwnerComponent) override;
	void ExecuteAction(const TArray<ABLCombatSlot*>& Targets) override;

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BL|Combat")
	TArray<FEnemiesSpawnData> EnemiesToSpawn;
};
