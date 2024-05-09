// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLEnemyEntryWidget.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "BLEnemyEntryData.h"
#include "UI/BLCooldownBarWidget.h"
#include "Components/ProgressBar.h"

void UBLEnemyEntryWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	UBLEnemyEntryData* Item = Cast<UBLEnemyEntryData>(ListItemObject);
	if (Item)
	{
		Index = Item->Index;
		FFormatNamedArguments Args;
		Args.Add(TEXT("NAME"), Item->Name);
		Args.Add(TEXT("LVL"), Item->Level);
		Name->SetText(FText::Format(FText::FromString("{NAME} {LVL}lv"), Args));
		Border->SetBrushColor(DefaultColor);
	}
}

void UBLEnemyEntryWidget::GreyOutEnemy()
{
	Border->SetBrushColor(DeathColor);
}

void UBLEnemyEntryWidget::UpdateHP(float CurrentHP, float MaxHP)
{
	const float Percent = CurrentHP / MaxHP;
	HPBar->SetPercent(Percent);
}
