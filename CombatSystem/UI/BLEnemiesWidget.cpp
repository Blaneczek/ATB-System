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
	for (auto Item : EnemiesTileView->GetListItems())
	{
		UBLEnemyEntryData* Enemy = Cast<UBLEnemyEntryData>(Item);
		if (Enemy && Enemy->Index == Index)
		{
			EnemiesTileView->RemoveItem(Item);
			return;
		}
	}
}

void UBLEnemiesWidget::UpdateEnemyHP(int32 Index, float CurrentHP, float MaxHP)
{
	for (const auto Enemy : EnemiesTileView->GetDisplayedEntryWidgets())
	{
		UBLEnemyEntryWidget* EnemyWidget = Cast<UBLEnemyEntryWidget>(Enemy);
		if (EnemyWidget && EnemyWidget->Index == Index)
		{
			EnemyWidget->UpdateHP(CurrentHP, MaxHP);
			return;
		}
	}
}

void UBLEnemiesWidget::EnemyDied(int32 Index)
{
	for (const auto Enemy: EnemiesTileView->GetDisplayedEntryWidgets())
	{
		UBLEnemyEntryWidget* EnemyWidget = Cast<UBLEnemyEntryWidget>(Enemy);
		if (EnemyWidget && EnemyWidget->Index == Index)
		{
			EnemyWidget->GreyOutEnemy();
			return;
		}
	}
}




