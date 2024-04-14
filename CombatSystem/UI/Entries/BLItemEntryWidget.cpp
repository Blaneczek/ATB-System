// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLItemEntryWidget.h"
#include "Components/Image.h"
#include "BLItemEntryData.h"

void UBLItemEntryWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	UBLItemEntryData* Item = Cast<UBLItemEntryData>(ListItemObject);
	if (Item)
	{
		SetData(Item->Index, Item->Thumbnail);
	}
}

void UBLItemEntryWidget::SetData(int32 InIndex, UTexture2D* InThumbnail)
{
	Index = InIndex;

	if (InThumbnail)
	{
		Thumbnail->SetBrushFromTexture(InThumbnail, true);
	}
	else
	{
		Thumbnail->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.f));
		SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	
}

