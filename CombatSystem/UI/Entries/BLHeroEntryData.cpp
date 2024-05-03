// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLHeroEntryData.h"

void UBLHeroEntryData::Init(int32 InIndex, const FText& InName, float InMaxHP, float InCurrentHP, float InMaxME)
{
	Index = InIndex;
	Name = InName;
	MaxHP = InMaxHP;
	CurrentHP = InCurrentHP;
	MaxME = InMaxME;
	CurrentME = InMaxME;
}
