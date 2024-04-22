// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLEnemyDataAsset.h"

void UBLEnemyDataAsset::SetData(const TArray<FEnemyAssetInfo>& InEnemies)
{
	Enemies = InEnemies;
}
