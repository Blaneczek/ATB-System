// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BLHeroesWidget.generated.h"

class UTileView;

/**
 * 
 */
UCLASS()
class BLADEOFLEGEND_API UBLHeroesWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	/** Adds item to TileView */
	void AddHero(int32 Index, const FString& HeroName, float HP, float ME);

	/** Will call when one of the heroes has died which will gray out the corresponding item in TileView */
	void HeroDied(int32 Index);

	/**  
	* Will call when one of the heroes has been selected or deselected which will change color of the corresponding item in TileView 
	* @param bNewColor true - highlight color, false - default color
	*/
	void HighlightHero(int32 Index, bool bNewColor);
	
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
	/** TileView with the heroes who will take part in the Combat */
	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat", meta = (BindWidget))
	TObjectPtr<UTileView> HeroesTileView;
};
