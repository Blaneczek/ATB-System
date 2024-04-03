// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BLCombatUtilities.h"
#include "BLCombatWidget.generated.h"

class UBLEnemiesWidget;
class UBLHeroesWidget;
class UBLActionsWidget;
class UWidgetSwitcher;
class UBorder;
class UBLAction;
class UTextBlock;
class UBLButtonEntryData;

DECLARE_DELEGATE_SixParams(FOnActionTypeChosen, ECombatActionType /*ActionType*/, int32 /*ActionIndex*/, ECrystalColor /*CrystalColor*/, float /*ActionMECost*/, int32 /*TargetsNum*/, UObject* /*ActionEntry*/);
DECLARE_DELEGATE_OneParam(FOnHeroSelected, int32 /*HeroIndex*/);

/**
 * 
 */
UCLASS()
class BLADEOFLEGEND_API UBLCombatWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	virtual void NativeOnInitialized() override;

public:
	/** Adds row with hero to widget */
	void AddHero(int32 SlotIndex, const FCombatCharData& BaseData);
	/** Adds cell with enemy to widget */
	void AddEnemy(int32 SlotIndex, const FString& EnemyName);
	/** Populate actions */
	void AddHeroActions(int32 SlotIndex, const FCombatCharData& BaseData, const TArray<TSoftClassPtr<UBLAction>>& InAttackActions, const TArray<TSoftClassPtr<UBLAction>>& InDefendActions, const TMap<ECrystalColor, FCrystalSkills>& InCrystalActions, const TArray<TSoftClassPtr<UBLAction>>& InSpecialActions);

	/** Highlights chosen hero and shows his actions */
	void SetCurrentHero(int32 SlotIndex);
	/** Unhighlights highlighted hero and hides his actions */
	void ResetCurrentHero(int32 SlotIndex);

	/** Called when one of the heroes died and greys him out */
	void HeroDied(int32 SlotIndex);
	/** Called when one of the enemies died and greys him out */
	void EnemyDied(int32 SlotIndex);

	/** Stops filling heroes' cooldown bars */
	void PauseCooldownBars(bool bNewPause);
	/** Starts filling from zero hero's cooldown bar */
	void StartHeroCooldownBar(int32 SlotIndex, float Cooldown);
	/** Resets hero's cooldown bar to zero */
	void ResetHeroCooldownBar(int32 SlotIndex);

	void SetIsActionVisibility(ESlateVisibility InVisibility);

	void UpdateHeroHealth(int32 SlotIndex, float MaxHP, float CurrentHP);
	void UpdateHeroMagicEnergy(int32 SlotIndex, float MaxME, float CurrentME);

	void HideActions();

	void ActivateNotEnoughME();

private:
	void ChosenAction(ECombatActionType Action, int32 ActionIndex, ECrystalColor CrystalColor = ECrystalColor::NONE, float ActionMECost = 0.f, int32 TargetsNum = 1, UObject* ActionEntry = nullptr);

	void HeroClicked(int32 HeroIndex);

public:
	FOnActionTypeChosen OnActionChosen;
	FOnHeroSelected OnHeroSelected;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat", meta = (BindWidget))
	TObjectPtr<UBLEnemiesWidget> Enemies;

	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat", meta = (BindWidget))
	TObjectPtr<UBLHeroesWidget> Heroes;

	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat", meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> ActionsSwitcher;

	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat", meta = (BindWidget))
	TObjectPtr<UBorder> NoneActions;
	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat", meta = (BindWidget))
	TObjectPtr<UBLActionsWidget> Actions_0;
	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat", meta = (BindWidget))
	TObjectPtr<UBLActionsWidget> Actions_1;
	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat", meta = (BindWidget))
	TObjectPtr<UBLActionsWidget> Actions_2;
	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat", meta = (BindWidget))
	TObjectPtr<UBLActionsWidget> Actions_3;
	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat", meta = (BindWidget))
	TObjectPtr<UBLActionsWidget> Actions_4;

	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat", meta = (BindWidget))
	TObjectPtr<UTextBlock> NotEnoughME;

private:
	UPROPERTY()
	TArray<TObjectPtr<UBLActionsWidget>> Actions;
};
