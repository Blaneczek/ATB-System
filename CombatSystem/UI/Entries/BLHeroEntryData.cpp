// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLHeroEntryData.h"

void UBLHeroEntryData::Init(int32 InIndex, const FText& InName, float InHP, float InME)
{
	Index = InIndex;
	Name = InName;
	HP = InHP;
	ME = InME;
}

void UBLHeroEntryData::HeroDied()
{
	OnHeroDied.ExecuteIfBound();
}
