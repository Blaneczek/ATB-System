// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLActionEntryWidget.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "BLActionEntryData.h"

void UBLActionEntryWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	UBLActionEntryData* Item = Cast<UBLActionEntryData>(ListItemObject);
	if (Item)
	{
		SetData(Item->Index, Item->Name, Item->MECost, Item->TargetsNum);
	}
}

void UBLActionEntryWidget::SetData(int32 InIndex, const FText& InName, float InMECost, int32 InTargetsNum)
{
	Index = InIndex;
	Name->SetText(InName);
	MECost = InMECost;
	TargetsNum = InTargetsNum;
}

void UBLActionEntryWidget::SetIsClicked(bool bNewClicked)
{
	if (bNewClicked)
	{
		Border->SetBrushColor(ClickedColor);
		Border->Background.OutlineSettings.Color = FLinearColor(0.f, 0.f, 0.f, 1.f);
	}
	else
	{
		Border->SetBrushColor(DefaultColor);
		Border->Background.OutlineSettings.Color = FLinearColor(1.f, 1.f, 1.f, 1.f);
	}
}
