// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "BLItemEntryWidget.generated.h"

class UImage;
class UBorder;

/**
 * 
 */
UCLASS()
class BLADEOFLEGEND_API UBLItemEntryWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	/** IUserObjectListEntry interface implementation */
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

public:
	void SetData(int32 InIndex, UTexture2D* InThumbnail);

public:
	UPROPERTY()
	int32 Index;

	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat", meta = (BindWidget))
	TObjectPtr<UImage> Thumbnail;

	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat", meta = (BindWidget))
	TObjectPtr<UBorder> Border;
};
