// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLButtonEntryWidget.h"
#include "Components/TextBlock.h"
#include "BLButtonEntryData.h"

void UBLButtonEntryWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	UBLButtonEntryData* Item = Cast<UBLButtonEntryData>(ListItemObject);
	if (Item)
	{
		SetData(Item->Index, Item->Name);
	}
}

void UBLButtonEntryWidget::SetData(int32 InIndex, const FText& InName)
{
	Index = InIndex;
	Name->SetText(InName);
}
