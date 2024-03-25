// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BLActionWidget.h"
#include "BLCrystalActionWidget.generated.h"

class UWidgetSwitcher;

USTRUCT(BlueprintType)
struct FCrystalDescriptions
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FText> Descriptions;
};

/**
 * 
 */
UCLASS()
class BLADEOFLEGEND_API UBLCrystalActionWidget : public UBLActionWidget
{
	GENERATED_BODY()

public:
	virtual void AddActions(const TMap<ECrystalColor, FCrystalSkills>& InCrystalActions) override;

	virtual void ResetAction() override;

protected:
	virtual void NativeConstruct() override;

	virtual void OnActionClicked(UObject* Item) override;

	void OnColorClicked(UObject* Item);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat", meta = (BindWidget))
	TObjectPtr<UListView> CrystalsList;

	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat", meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> ColorsSwitcher;

private:
	UPROPERTY()
	TMap<ECrystalColor, FCrystalDescriptions> CrystalsDescriptions;

	UPROPERTY()
	TArray<TObjectPtr<UObject>> RedItems;
	UPROPERTY()
	TArray<TObjectPtr<UObject>> BlueItems;

	UPROPERTY()
	ECrystalColor ClickedColor{ ECrystalColor::NONE };

	/** Clicked item in Crystals ListView */
	UPROPERTY()
	TObjectPtr<UBLButtonEntryWidget> ClickedColorButton;
};
