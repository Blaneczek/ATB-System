// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "BLButtonEntryWidget.generated.h"

class UBorder;
class UTextBlock;

/**
 * 
 */
UCLASS()
class BLADEOFLEGEND_API UBLButtonEntryWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
protected:
	/** IUserObjectListEntry interface implementation */
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

public:
	void SetData(int32 InIndex, const FText& InName, float InMECost = 0.f, int32 InTargetsNum = 1);

public:
	UPROPERTY()
	int32 Index;

	UPROPERTY()
	float MECost;

	UPROPERTY()
	int32 TargetsNum;

	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat", meta = (BindWidget))
	TObjectPtr<UBorder> Border;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat", meta = (BindWidget))
	TObjectPtr<UTextBlock> Name;

	
};
