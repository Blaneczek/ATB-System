// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BLButtonEntryData.generated.h"

/**
 * 
 */
UCLASS()
class BLADEOFLEGEND_API UBLButtonEntryData : public UObject
{
	GENERATED_BODY()
	
public:
	/** Sets the data needed for UI */
	void Init(int32 InIndex, const FText& InName);

public:
	UPROPERTY()
	int32 Index;
	UPROPERTY()
	FText Name;
};
