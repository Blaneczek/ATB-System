// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BLCombatUtilities.h"
#include "BLCombatWidget.generated.h"

class UBLEnemiesWidget;
class UBLHeroesWidget;


DECLARE_DELEGATE_TwoParams(FOnChosenAction, ECombatActionType /*ActionType*/, int32 /*ActionIndex*/);

/**
 * 
 */
UCLASS()
class BLADEOFLEGEND_API UBLCombatWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	void AddHero(int32 SlotIndex, const FCombatCharData& BaseData);
	void AddEnemy(int32 SlotIndex, const FString& EnemyName);

	void SetCurrentHero(int32 SlotIndex);

	void ShowActions(int32 SlotIndex);
	void ResetActions(int32 SlotIndex);

	void HighlightHero(int32 SlotIndex);
	void UnlightHero(int32 SlotInde);
	void HeroDied(int32 SlotIndex);
	void EnemyDied(int32 SlotIndex);

	void PauseCooldownBars();
	void UnPauseCooldownBars();
	void StartHeroCooldownBar(int32 SlotIndex, float Cooldown);
	void ResetHeroCooldownBar(int32 SlotIndex);
	
	void SetIsActionVisibility(ESlateVisibility InVisibility);

	void UpdateHeroHealth(int32 SlotIndex, float CurrentHP, float MaxHP);

public:
	FOnChosenAction OnChosenAction;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat", meta = (BindWidget))
	TObjectPtr<UBLEnemiesWidget> Enemies;

	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat", meta = (BindWidget))
	TObjectPtr<UBLHeroesWidget> Heroes;
};
