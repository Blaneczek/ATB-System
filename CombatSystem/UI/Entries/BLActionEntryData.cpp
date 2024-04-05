// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLActionEntryData.h"

void UBLActionEntryData::Init(int32 InIndex, const FText& InName, ECrystalColor InCrystalColor, float InMECost, int32 InTurnsCost, int32 InTargetsNum)
{
	Index = InIndex;
	Name = InName;
	CrystalColor = InCrystalColor;
	MECost = InMECost;
	TurnsCost = InTurnsCost;
	TargetsNum = InTargetsNum;

	TempName = InName;
}

void UBLActionEntryData::ChangeName(const FText& NewName)
{
	Name = NewName;
	OnNameChange.ExecuteIfBound(this);
}
