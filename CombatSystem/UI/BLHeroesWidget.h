// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BLHeroesWidget.generated.h"

class UTileView;
class UBLHeroEntryWidget;


DECLARE_DELEGATE_OneParam(FOnHeroClicked, int32 /*HeroIndex*/);

/**
 * 
 */
UCLASS()
class BLADEOFLEGEND_API UBLHeroesWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/** Adds item to TileView */
	void AddHero(int32 Index, const FString& HeroName, float HP, float ME);

	/** Will call when one of the heroes has died which will gray out the corresponding item in TileView */
	void HeroDied(int32 Index);

	/**  
	* Will call when one of the heroes has been selected or deselected which will change color of the corresponding item in TileView 
	* @param bNewColor true - highlight color, false - default color
	*/
	void HighlightsHero(int32 Index, bool bNewColor);

	void UnlightsHero();

	void UpdateHeroHP(int32 Index, float MaxHP, float CurrentHP);
	void UpdateHeroME(int32 Index, float MaxME, float CurrentME);
	void StartHeroCooldownBar(int32 Index, float Cooldown);
	void ResetHeroCooldownBar(int32 Index);

	/**
	* Pause/unpause all cooldown bars 
	* @param bNewPause true - pause, false - unpause
	*/
	void PauseAllCooldownBars(bool bNewPause);

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void HeroClicked(UObject* Item);
	
public:
	FOnHeroClicked OnHeroClicked;

protected:
	/** TileView with the heroes who will take part in the Combat */
	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat", meta = (BindWidget))
	TObjectPtr<UTileView> HeroesTileView;

private:
	/** Current clicked Hero in HeroesTileView */
	UPROPERTY()
	TObjectPtr<UBLHeroEntryWidget> ClickedHero;
};
