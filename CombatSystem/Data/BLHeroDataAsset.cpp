// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLHeroDataAsset.h"

FHeroDataAssetData UBLHeroDataAsset::GetDataAssetAsStruct()
{
	return FHeroDataAssetData(Heroes, Level, Experience, ExperienceNextLevel, Items, Money);
}

void UBLHeroDataAsset::SetDataAssetFromStruct(const FHeroDataAssetData& Data)
{
	Heroes = Data.Heroes;
	Level = Data.Level;
	Experience = Data.Experience;
	ExperienceNextLevel = Data.ExperienceNextLevel;
	Items = Data.Items;
	Money = Data.Money;
}


