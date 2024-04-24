// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BLActionWidget.h"
#include "BLItemActionWidget.generated.h"

class UTileView;
class UBLItemEntryWidget;
class UBLCombatItem;

/**
 * 
 */
UCLASS()
class BLADEOFLEGEND_API UBLItemActionWidget : public UBLActionWidget
{
	GENERATED_BODY()

public:
	virtual void AddActions(const TArray<TSoftClassPtr<UBLCombatItem>>& InItemsActions) override;

	virtual void ResetAction() override;

protected:
	virtual void OnActionClicked(UObject* Item) override;

	virtual void NativeConstruct() override;

private:
	void DeleteItem(int32 Index);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat", meta = (BindWidget))
	TObjectPtr<UTileView> ItemsList;

	UPROPERTY()
	TObjectPtr<UBLItemEntryWidget> ClickedItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor ClickedColor;
};
