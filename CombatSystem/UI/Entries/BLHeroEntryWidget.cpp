// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLHeroEntryWidget.h"
#include "BLHeroEntryData.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "UI/BLCooldownBarWidget.h"


void UBLHeroEntryWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	UBLHeroEntryData* Item = Cast<UBLHeroEntryData>(ListItemObject);
	if (Item)
	{
		SetData(Item->Index, Item->Name, Item->MaxHP, Item->CurrentHP, Item->MaxME, Item->CurrentME);
	}
}

void UBLHeroEntryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CooldownBar->OnCooldownEnded.BindLambda([this](){ bCanDoAction = true; });
}

void UBLHeroEntryWidget::SetData(int32 InIndex, const FText& InName, float InMaxHP, float InCurrentHP, float InMaxME, float InCurrentME)
{
	Index = InIndex;
	Name->SetText(InName);
	FFormatNamedArguments Args;
	Args.Add(TEXT("maxHP"), InMaxHP);
	Args.Add(TEXT("currentHP"), InCurrentHP);
	Args.Add(TEXT("maxME"), InMaxME);
	Args.Add(TEXT("currentME"), InCurrentME);
	HP->SetText(FText::Format(FText::FromString("HP: {currentHP}/{maxHP}"), Args));
	ME->SetText(FText::Format(FText::FromString("ME: {currentME}/{maxME}"), Args));
}

void UBLHeroEntryWidget::GreyOutHero()
{
	bDied = true;
	Border->SetBrushColor(DeathColor);
	ResetCooldownBar();
}

void UBLHeroEntryWidget::HighlightHero()
{
	Border->SetBrushColor(HighlightColor);
}

void UBLHeroEntryWidget::UnlightHero()
{
	Border->SetBrushColor(DefaultColor);
}

void UBLHeroEntryWidget::UpdateHP(float MaxHP, float CurrentHP)
{
	FFormatNamedArguments Args;
	Args.Add(TEXT("MaxHP"), MaxHP);
	Args.Add(TEXT("CurrentHP"), CurrentHP);
	HP->SetText(FText::Format(FText::FromString("HP: {CurrentHP}/{MaxHP}"), Args));
}

void UBLHeroEntryWidget::UpdateME(float MaxME, float CurrentME)
{
	FFormatNamedArguments Args;
	Args.Add(TEXT("MaxME"), MaxME);
	Args.Add(TEXT("CurrentME"), CurrentME);
	ME->SetText(FText::Format(FText::FromString("ME: {CurrentME}/{MaxME}"), Args));
}

void UBLHeroEntryWidget::StartCooldownBar(float Cooldown)
{
	bCanDoAction = false;
	CooldownBar->StartCooldown(Cooldown);
}

void UBLHeroEntryWidget::PauseCooldownBar()
{
	CooldownBar->PauseCooldown();
}

void UBLHeroEntryWidget::UnPauseCooldownBar()
{
	CooldownBar->UnPauseCooldown();
}

void UBLHeroEntryWidget::ResetCooldownBar()
{
	bCanDoAction = false;
	CooldownBar->ResetCooldown();
}
