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
	FCombatCharData BaseData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BL|Combat")
	TArray<TSoftClassPtr<UBLAction>> Actions;

	FEnemiesSpawnData()
	{}

	FEnemiesSpawnData(const FCombatCharData& InBaseData, const TArray<TSoftClassPtr<UBLAction>>& InActions)
		: BaseData(InBaseData), Actions(InActions)
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
	void ActivateAction(ABLCombatCharacter* Owner, const TArray<ABLCombatSlot*>& Targets) override;
	void ExecuteAction(ABLCombatCharacter* Owner, const TArray<ABLCombatSlot*>& Targets) override;

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BL|Combat")
	TArray<FEnemiesSpawnData> EnemiesToSpawn;
};
