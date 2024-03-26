// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLButtonEntryData.h"

void UBLButtonEntryData::Init(int32 InIndex, const FText& InName, ECrystalColor InCrystalColor, float InMECost)
{
	Index = InIndex;
	Name = InName;
	CrystalColor = InCrystalColor;
	MECost = InMECost;
}