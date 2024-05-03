// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLEnemyEntryData.h"

void UBLEnemyEntryData::Init(int32 InIndex, const FText& InName, int32 InLevel)
{
	Index = InIndex;
	Name = InName;
	Level = InLevel;
}
