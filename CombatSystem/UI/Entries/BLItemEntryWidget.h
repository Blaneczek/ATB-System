// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "BLItemEntryWidget.generated.h"

class UImage;

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
	void SetData(const FText& InName, UTexture2D* Texture);

public:
	UPROPERTY()
	FText Name;

	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat", meta = (BindWidget))
	TObjectPtr<UImage> Image;
};
