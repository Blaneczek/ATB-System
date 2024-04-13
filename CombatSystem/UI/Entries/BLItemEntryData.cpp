// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLItemEntryData.h"

void UBLItemEntryData::Init(int32 InIndex, const FText& InName, ECombatActionFlow InActionFlow, UTexture2D* InThumbnail)
{
	Index = InIndex;
	Name = InName;
	ActionFlow = InActionFlow;
	Thumbnail = InThumbnail;
}
