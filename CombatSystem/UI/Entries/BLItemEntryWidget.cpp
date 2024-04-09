// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLItemEntryWidget.h"
#include "Components/Image.h"
#include "BLItemEntryData.h"

void UBLItemEntryWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	UBLItemEntryData* Item = Cast<UBLItemEntryData>(ListItemObject);
	if (Item)
	{
		SetData(Item->Name, Item->Texture);
	}
}

void UBLItemEntryWidget::SetData(const FText& InName, UTexture2D* Texture)
{
	Name = InName;
	Image->SetBrushFromTexture(Texture, true);
}
