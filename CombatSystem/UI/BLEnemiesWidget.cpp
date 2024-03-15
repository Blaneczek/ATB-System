// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLEnemiesWidget.h"
#include "Components/TileView.h"
#include "Entries/BLEnemyEntryData.h"

void UBLEnemiesWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void UBLEnemiesWidget::EnemyDied(int32 Index)
{
	UBLEnemyEntryData* Enemy= Cast<UBLEnemyEntryData>(EnemiesTileView->GetItemAt(Index));
	if (Enemy)
	{
		Enemy->EnemyDied();
	}
}

void UBLEnemiesWidget::AddEnemy(int32 Index, const FString& EnemyName)
{
	UBLEnemyEntryData* Enemy = NewObject<UBLEnemyEntryData>();
	if (Enemy)
	{
		Enemy->Init(Index, FText::FromString(EnemyName));
		EnemiesTileView->AddItem(Enemy);
	}
}


