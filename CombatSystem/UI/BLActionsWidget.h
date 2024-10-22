// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/BLCombatUtilities.h"
#include "BLActionsWidget.generated.h"

class UButton;
class UBorder;
class UTextBlock;
class UWidgetSwitcher;
class UBLActionWidget;
class UBLButtonEntryData;

DECLARE_DELEGATE_OneParam(FOnChosenAction, const FCombatActionData& ActionData);
DECLARE_DELEGATE(FOnResetActionType);

/**
 * 
 */
UCLASS()
class BLADEOFLEGEND_API UBLActionsWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	void SetActionsData(const FCombatCharData& BaseData, const FCombatActions& CombatActions, bool bCanRunAway);

	/** Returns widget to default state */
	void ResetWidget();

private:
	void BindButtons();
	void BindActions();

	void DisableButton(UButton* Button);

	UFUNCTION()
	void OnBTAttackClicked();
	UFUNCTION()
	void OnBTDefendClicked();
	UFUNCTION()
	void OnBTCrystalClicked();
	UFUNCTION()
	void OnBTSpecialSkillClicked();
	UFUNCTION()
	void OnBTItemClicked();
	UFUNCTION()
	void OnBTRunClicked();

	/** Returns clicked button to normal color */
	void ResetButton();
	/** Returns clicked action to default state */
	void ResetAction();
	
	/** Calls delegate with chosen action and index */
	void ChosenAction(const FCombatActionData& ActionData);

public:
	FOnChosenAction OnChosenAction;
	FOnResetActionType OnResetActionType;

protected:
	/** Action type buttons */
	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat", meta = (BindWidget))
	TObjectPtr<UButton> BTAttack;
	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat", meta = (BindWidget))
	TObjectPtr<UButton> BTDefend;
	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat", meta = (BindWidget))
	TObjectPtr<UButton> BTCrystal;
	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat", meta = (BindWidget))
	TObjectPtr<UButton> BTSpecial;
	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat", meta = (BindWidget))
	TObjectPtr<UButton> BTItem;
	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat", meta = (BindWidget))
	TObjectPtr<UButton> BTRun;

	/** Switcher for different type of actions  */
	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat", meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> ActionTypeSwitcher;

	/** Actions to display*/
	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat", meta = (BindWidget))
	TObjectPtr<UBorder> NoneAction;

	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat", meta = (BindWidget))
	TObjectPtr<UBLActionWidget> AttackAction;

	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat", meta = (BindWidget))
	TObjectPtr<UBLActionWidget> DefendAction;

	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat", meta = (BindWidget))
	TObjectPtr<UBLActionWidget> CrystalAction;

	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat", meta = (BindWidget))
	TObjectPtr<UBLActionWidget> SpecialAction;

	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat", meta = (BindWidget))
	TObjectPtr<UBLActionWidget> ItemAction;

	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat", meta = (BindWidget))
	TObjectPtr<UBLActionWidget> RunAwayAction;

	/** Name of special actions to display on button */
	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat", meta = (BindWidget))
	TObjectPtr<UTextBlock> SpecialActionsName;

private:
	UPROPERTY()
	TObjectPtr<UButton> ClickedButton;
	UPROPERTY()
	TObjectPtr<UBLActionWidget> ClickedAction;
};
