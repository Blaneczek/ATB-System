// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLEnemyEntryWidget.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "BLEnemyEntryData.h"

void UBLEnemyEntryWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	UBLEnemyEntryData* Item = Cast<UBLEnemyEntryData>(ListItemObject);
	if (Item)
	{
		Name->SetText(Item->Name);
		Item->OnEnemyDied.BindUObject(this, &UBLEnemyEntryWidget::EnemyDied);
	}
}

void UBLEnemyEntryWidget::EnemyDied()
{
	Border->SetBrushColor(FLinearColor(0.3f, 0.3f, 0.3f, 1.f));
}
