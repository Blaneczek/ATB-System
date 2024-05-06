// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLEnemiesWidget.h"
#include "Components/TileView.h"
#include "Entries/BLEnemyEntryData.h"
#include "Entries/BLEnemyEntryWidget.h"

void UBLEnemiesWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void UBLEnemiesWidget::AddEnemy(int32 Index, const FString& EnemyName, int32 Level)
{
	UBLEnemyEntryData* Enemy = NewObject<UBLEnemyEntryData>();
	if (Enemy)
	{
		Enemy->Init(Index, FText::FromString(EnemyName), Level);
		EnemiesTileView->AddItem(Enemy);
	}
}

void UBLEnemiesWidget::RemoveEnemy(int32 Index)
{
	EnemiesTileView->RemoveItem(EnemiesTileView->GetItemAt(Index));
}

void UBLEnemiesWidget::EnemyDied(int32 Index)
{
	UBLEnemyEntryWidget* EnemyWidget = Cast<UBLEnemyEntryWidget>(EnemiesTileView->GetEntryWidgetFromItem(EnemiesTileView->GetItemAt(Index)));
	if (EnemyWidget)
	{
		EnemyWidget->GreyOutEnemy();
	}
}




