// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BLEnemiesWidget.generated.h"

class UTileView;

/**
 * 
 */
UCLASS()
class BLADEOFLEGEND_API UBLEnemiesWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	/** Will call when one of the Enemies has died which will gray out the corresponding item in TileView */
	void EnemyDied(int32 Index);

	/** Adds item to TileView */
	void AddEnemy(int32 Index, const FString& EnemyName, int32 Level);

	/** Removes item from TileView */
	void RemoveEnemy(int32 Index);
 
	/**
	* Pause/unpause all cooldown bars
	* @param bNewPause true - pause, false - unpause
	*/
	void PauseAllCooldownBars(bool bNewPause);

	void StartEnemyCooldownBar(int32 Index, float Cooldown);

	void ResetEnemyCooldownBar(int32 Index);

protected:
	/** TileView with the enemies who will take part in the Combat */
	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat", meta = (BindWidget))
	TObjectPtr<UTileView> EnemiesTileView;

};
