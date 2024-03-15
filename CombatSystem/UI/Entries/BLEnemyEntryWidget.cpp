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
	}
}

void UBLEnemyEntryWidget::GreyOutEnemy()
{
	Border->SetBrushColor(FLinearColor(0.3f, 0.3f, 0.3f, 1.f));
}
