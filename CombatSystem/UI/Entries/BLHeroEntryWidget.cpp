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
		SetData(Item->Index, Item->Name, Item->HP, Item->ME);
	}
}

void UBLHeroEntryWidget::SetData(int32 InIndex, const FText& InName, float InHP, float InME)
{
	Index = InIndex;
	Name->SetText(InName);
	FFormatNamedArguments Args;
	Args.Add(TEXT("HP"), InHP);
	Args.Add(TEXT("ME"), InME);
	HP->SetText(FText::Format(FText::FromString("HP: {HP}/{HP}"), Args));
	ME->SetText(FText::Format(FText::FromString("ME: {ME}/{ME}"), Args));
}
