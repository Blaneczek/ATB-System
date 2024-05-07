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
	RemoveEnemy(Index);

	UBLEnemyEntryData* Enemy = NewObject<UBLEnemyEntryData>();
	if (Enemy)
	{
		Enemy->Init(Index, FText::FromString(EnemyName), Level);
		EnemiesTileView->AddItem(Enemy);
		UE_LOG(LogTemp, Warning, TEXT("added"));
	}
}

void UBLEnemiesWidget::RemoveEnemy(int32 Index)
{		
	for (auto Item : EnemiesTileView->GetListItems())
	{
		UBLEnemyEntryData* Enemy = Cast<UBLEnemyEntryData>(Item);
		if (Enemy && Enemy->Index == Index)
		{
			UE_LOG(LogTemp, Warning, TEXT("delete"));
			EnemiesTileView->RemoveItem(Item);
			return;
		}
	}
}

void UBLEnemiesWidget::EnemyDied(int32 Index)
{
	for (auto Enemy: EnemiesTileView->GetDisplayedEntryWidgets())
	{
		UBLEnemyEntryWidget* EnemyWidget = Cast<UBLEnemyEntryWidget>(Enemy);
		if (EnemyWidget && EnemyWidget->Index == Index)
		{
			EnemyWidget->GreyOutEnemy();
			return;
		}
	}
}




