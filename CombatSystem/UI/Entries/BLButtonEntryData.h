// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BLCombatUtilities.h"
#include "BLButtonEntryData.generated.h"

/**
 * 
 */
UCLASS()
class BLADEOFLEGEND_API UBLButtonEntryData : public UObject
{
	GENERATED_BODY()
	
public:
	/** Sets the data needed for UI */
	void Init(int32 InIndex, const FText& InName, ECrystalColor InCrystalColor = ECrystalColor::NONE, float InMECost = 0.f, int32 InTurnsCost = 0, int32 InTargetsNum = 1);

public:
	UPROPERTY()
	int32 Index;
	UPROPERTY()
	FText Name;
	UPROPERTY()
	ECrystalColor CrystalColor;
	UPROPERTY()
	float MECost;
	UPROPERTY()
	int32 TurnsCost;
	UPROPERTY()
	int32 TargetsNum;

	UPROPERTY()
	bool bCanBeUsed{ true };
};
