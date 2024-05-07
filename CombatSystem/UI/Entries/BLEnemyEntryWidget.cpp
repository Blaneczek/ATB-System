// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLEnemyEntryWidget.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "BLEnemyEntryData.h"
#include "UI/BLCooldownBarWidget.h"

void UBLEnemyEntryWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	UBLEnemyEntryData* Item = Cast<UBLEnemyEntryData>(ListItemObject);
	if (Item)
	{
		bDied = false;
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
	bDied = true;
}

void UBLEnemyEntryWidget::StartCooldownBar(float Cooldown)
{
	if (bDied) return;

	CooldownBar->StartCooldown(Cooldown);
}

void UBLEnemyEntryWidget::PauseCooldownBar()
{
	CooldownBar->PauseCooldown();
}

void UBLEnemyEntryWidget::UnPauseCooldownBar()
{
	if (bDied)
	{
		ResetCooldownBar();
		return;
	}
	CooldownBar->UnPauseCooldown();
}

void UBLEnemyEntryWidget::ResetCooldownBar()
{
	CooldownBar->ResetCooldown();
}
