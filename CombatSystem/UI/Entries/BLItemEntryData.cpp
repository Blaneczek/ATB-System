// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLItemEntryData.h"

void UBLItemEntryData::Init(const FText& InName, UTexture2D* InTexture)
{
	Name = InName;
	Texture = InTexture;
}
