// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BLEnemyEntryData.generated.h"

/**
 * 
 */
UCLASS()
class BLADEOFLEGEND_API UBLEnemyEntryData : public UObject
{
	GENERATED_BODY()

public:
	/** Sets the data needed for UI */
	void Init(int32 InIndex, const FText& InName, int32 InLevel);

public:
	UPROPERTY()
	int32 Index;

	UPROPERTY()
	FText Name;

	UPROPERTY()
	int32 Level;

};
