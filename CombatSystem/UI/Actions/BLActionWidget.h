// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BLCombatUtilities.h"
#include "BLActionWidget.generated.h"

class UTextBlock;
class UButton;
class UBLAction;

DECLARE_DELEGATE_TwoParams(FOnAction, ECombatActionType /*ActionType*/, int32 /*ActionIndex*/);

/**
 * 
 */
UCLASS()
class BLADEOFLEGEND_API UBLActionWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	virtual void OnBTActionClicked();

public:
	virtual void SetActionData(const TArray<TSoftClassPtr<UBLAction>>& InAttackActions, float AttackDMG) {};
	virtual void SetActionData(const TArray<TSoftClassPtr<UBLAction>>& InDefendActions) {};

	/** Resets action to default state (no description and normal button) */
	void ResetAction();

public:
	FOnAction OnAction;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat", meta = (BindWidget))
	TObjectPtr<UButton> BTAction;

	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat", meta = (BindWidget))
	TObjectPtr<UTextBlock> ActionName;

	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat", meta = (BindWidget))
	TObjectPtr<UTextBlock> DescDisplay;

	UPROPERTY()
	FText Description;

};
