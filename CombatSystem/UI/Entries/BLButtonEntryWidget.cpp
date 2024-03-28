// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLButtonEntryWidget.h"
#include "Components/TextBlock.h"
#include "BLButtonEntryData.h"

void UBLButtonEntryWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	UBLButtonEntryData* Item = Cast<UBLButtonEntryData>(ListItemObject);
	if (Item)
	{
		SetData(Item->Index, Item->Name, Item->MECost, Item->TargetsNum);
	}
}

void UBLButtonEntryWidget::SetData(int32 InIndex, const FText& InName, float InMECost, int32 InTargetsNum)
{
	Index = InIndex;
	Name->SetText(InName);
	MECost = InMECost;
	TargetsNum = InTargetsNum;
}
