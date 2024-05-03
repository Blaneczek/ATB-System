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
		FFormatNamedArguments Args;
		Args.Add(TEXT("NAME"), Item->Name);
		Args.Add(TEXT("LVL"), Item->Level);
		Name->SetText(FText::Format(FText::FromString("{NAME} {LVL}lv"), Args));
	}
}

void UBLEnemyEntryWidget::GreyOutEnemy()
{
	Border->SetBrushColor(DeathColor);
}
