// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BLHeroEntryData.generated.h"

DECLARE_DELEGATE(FOnHeroDied);

/**
 * 
 */
UCLASS()
class BLADEOFLEGEND_API UBLHeroEntryData : public UObject
{
	GENERATED_BODY()

public:
	void Init(int32 InIndex, const FText& InName, float InHP, float InME);

	void HeroDied();

public:
	FOnHeroDied OnHeroDied;

	UPROPERTY()
	int32 Index;
	UPROPERTY()
	FText Name;
	UPROPERTY()
	float HP;
	UPROPERTY()
	float ME;
};
