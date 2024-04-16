// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BLWinCombatWidget.generated.h"

class UButton;
class UTextBlock;
class UGridPanel;
class UBLItem;

DECLARE_DELEGATE(FOnWinGame);

/**
 * 
 */
UCLASS()
class BLADEOFLEGEND_API UBLWinCombatWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void OnBTNextClicked();

	void SetData(int32 InExp, int32 InMoney, const TArray<TSoftClassPtr<UBLItem>>& InItems);

public:
	FOnWinGame OnWinGame;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat", meta = (BindWidget))
	TObjectPtr<UButton> BTNext;

	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat", meta = (BindWidget))
	TObjectPtr<UTextBlock> Exp;

	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat", meta = (BindWidget))
	TObjectPtr<UTextBlock> Money;

	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat", meta = (BindWidget))
	TObjectPtr<UGridPanel> Items;
};
