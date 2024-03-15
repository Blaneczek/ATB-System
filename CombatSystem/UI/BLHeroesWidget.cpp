// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLHeroesWidget.h"
#include "Components/TileView.h"
#include "Entries/BLHeroEntryData.h"

void UBLHeroesWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UBLHeroesWidget::HeroDied(int32 Index)
{
	UBLHeroEntryData* Hero = Cast<UBLHeroEntryData>(HeroesTileView->GetItemAt(Index));
	if (Hero)
	{
		Hero->HeroDied();
	}
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


