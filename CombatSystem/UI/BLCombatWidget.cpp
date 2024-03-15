// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLCombatWidget.h"
#include "BLEnemiesWidget.h"
#include "BLHeroesWidget.h"

void UBLCombatWidget::NativeConstruct()
{
}

void UBLCombatWidget::AddHero(int32 SlotIndex, const FCombatCharData& BaseData)
{
	Heroes->AddHero(SlotIndex, BaseData.Name, BaseData.MaxHP, BaseData.MaxME);
}

void UBLCombatWidget::AddEnemy(int32 SlotIndex, const FString& EnemyName)
{
	Enemies->AddEnemy(SlotIndex, EnemyName);
}

void UBLCombatWidget::SetCurrentHero(int32 SlotIndex)
{
}

void UBLCombatWidget::ShowActions(int32 SlotIndex)
{
}

void UBLCombatWidget::ResetActions(int32 SlotIndex)
{
}

void UBLCombatWidget::HighlightHero(int32 SlotIndex)
{
}

void UBLCombatWidget::UnlightHero(int32 SlotInde)
{
}

void UBLCombatWidget::HeroDied(int32 SlotIndex)
{
}

void UBLCombatWidget::EnemyDied(int32 SlotIndex)
{
}

void UBLCombatWidget::PauseCooldownBars()
{
}

void UBLCombatWidget::UnPauseCooldownBars()
{
}

void UBLCombatWidget::StartHeroCooldownBar(int32 SlotIndex, float Cooldown)
{
}

void UBLCombatWidget::ResetHeroCooldownBar(int32 SlotIndex)
{
}

void UBLCombatWidget::SetIsActionVisibility(ESlateVisibility InVisibility)
{
}

void UBLCombatWidget::UpdateHeroHealth(int32 SlotIndex, float CurrentHP, float MaxHP)
{
}
