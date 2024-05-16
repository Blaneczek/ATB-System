// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/BLCombatUtilities.h"
#include "BLActionEntryData.generated.h"

DECLARE_DELEGATE_OneParam(FOnNameChange, UBLActionEntryData* /*ActionEntry*/);

/**
 * 
 */
UCLASS()
class BLADEOFLEGEND_API UBLActionEntryData : public UObject
{
	GENERATED_BODY()
	
public:
	/** Sets the data needed for UI */
	void Init(int32 InIndex, const FText& InName, ECombatActionFlow InActionFlow, ECrystalColor InCrystalColor = ECrystalColor::NONE, float InMECost = 0.f, int32 InTurnsCost = 0, int32 InTargetsNum = 1);

	/** If action is on turn cooldown, adds the number of remaining turns to the action name. */
	void ChangeName(int32 TurnNum);

	/** Goes back to default action name. */
	void ChangeNameToDefault();

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
	ECombatActionFlow ActionFlow;

	UPROPERTY()
	bool bCanBeUsed{ true };
	UPROPERTY()
	FText TempName;

	FOnNameChange OnNameChange;
};
