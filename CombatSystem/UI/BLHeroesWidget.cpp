// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLHeroesWidget.h"
#include "Components/TileView.h"
#include "Entries/BLHeroEntryWidget.h"
#include "Entries/BLHeroEntryData.h"

void UBLHeroesWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UBLHeroesWidget::AddHero(int32 Index, const FString& HeroName, float HP, float ME)
{
	UBLHeroEntryData* Hero = NewObject<UBLHeroEntryData>();
	if (Hero)
	{
		Hero->Init(Index, FText::FromString(HeroName), HP, ME);
		HeroesTileView->AddItem(Hero);
	}
}

void UBLHeroesWidget::HeroDied(int32 Index)
{
	UBLHeroEntryWidget* HeroWidget = Cast<UBLHeroEntryWidget>(HeroesTileView->GetEntryWidgetFromItem(HeroesTileView->GetItemAt(Index)));
	if (HeroWidget)
	{
		HeroWidget->GreyOutHero();
	}
}

void UBLHeroesWidget::HighlightHero(int32 Index, bool bNewColor)
{
	UBLHeroEntryWidget* HeroWidget = Cast<UBLHeroEntryWidget>(HeroesTileView->GetEntryWidgetFromItem(HeroesTileView->GetItemAt(Index)));
	if (HeroWidget)
	{
		bNewColor ? HeroWidget->HighlightHero() : HeroWidget->UnlightHero();
	}
}

void UBLHeroesWidget::UpdateHeroHP(int32 Index, float MaxHP, float CurrentHP)
{
	UBLHeroEntryWidget* HeroWidget = Cast<UBLHeroEntryWidget>(HeroesTileView->GetEntryWidgetFromItem(HeroesTileView->GetItemAt(Index)));
	if (HeroWidget)
	{
		HeroWidget->UpdateHP(MaxHP, CurrentHP);
	}
}

void UBLHeroesWidget::UpdateHeroME(int32 Index, float MaxME, float CurrentME)
{
	UBLHeroEntryWidget* HeroWidget = Cast<UBLHeroEntryWidget>(HeroesTileView->GetEntryWidgetFromItem(HeroesTileView->GetItemAt(Index)));
	if (HeroWidget)
	{
		HeroWidget->UpdateME(MaxME, CurrentME);
	}
}

void UBLHeroesWidget::StartHeroCooldownBar(int32 Index, float Cooldown)
{
	UBLHeroEntryWidget* HeroWidget = Cast<UBLHeroEntryWidget>(HeroesTileView->GetEntryWidgetFromItem(HeroesTileView->GetItemAt(Index)));
	if (HeroWidget)
	{
		HeroWidget->StartCooldownBar(Cooldown);
	}
}

void UBLHeroesWidget::PauseAllCooldownBars(bool bNewPause)
{
	TArray<UUserWidget*> Heroes = HeroesTileView->GetDisplayedEntryWidgets();
	for (auto* Hero : Heroes)
	{
		UBLHeroEntryWidget* HeroWidget = Cast<UBLHeroEntryWidget>(Hero);
		if (HeroWidget && !HeroWidget->IsDead())
		{
			bNewPause ? HeroWidget->PauseCooldownBar() : HeroWidget->UnPauseCooldownBar();
		}
	}
}

void UBLHeroesWidget::ResetHeroCooldownBar(int32 Index)
{
	UBLHeroEntryWidget* HeroWidget = Cast<UBLHeroEntryWidget>(HeroesTileView->GetEntryWidgetFromItem(HeroesTileView->GetItemAt(Index)));
	if (HeroWidget)
	{
		HeroWidget->ResetCooldownBar();
	}
}


