// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BLEnemyEntryData.generated.h"

DECLARE_DELEGATE(FOnEnemyDied);

/**
 * 
 */
UCLASS()
class BLADEOFLEGEND_API UBLEnemyEntryData : public UObject
{
	GENERATED_BODY()
	
public:
	void Init(int32 InIndex, const FText& InName);

	void EnemyDied();

public:
	FOnEnemyDied OnEnemyDied;

	UPROPERTY()
	int32 Index;
	UPROPERTY()
	FText Name;
};
