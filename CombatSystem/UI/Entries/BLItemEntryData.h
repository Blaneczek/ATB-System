// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BLItemEntryData.generated.h"

class UTexture2D;

/**
 * 
 */
UCLASS()
class BLADEOFLEGEND_API UBLItemEntryData : public UObject
{
	GENERATED_BODY()

public:
	void Init(const FText& InName, UTexture2D* Texture);

public:
	UPROPERTY()
	FText Name;

	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat", meta = (BindWidget))
	TObjectPtr<UTexture2D> Texture;
};
