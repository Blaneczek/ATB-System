// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BLCooldownBarWidget.generated.h"

class UProgressBar;

/**
 *
 * 
 */
UCLASS()
class BLADEOFLEGEND_API UBLCooldownBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	/** Starts filling bar from zero */
	void StartCooldown(float Cooldown);
	/** Resets bar to zero */
	void ResetCooldown();
	/** Stops filling bar */
	void PauseCooldown();
	/** Starts filling stopped bar */
	void UnPauseCooldown();

private:
	/** Increases bar value by TimerInterval */
	void UpdateBar(float Cooldown);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat", meta = (BindWidget))
	TObjectPtr<UProgressBar> CooldownBar;

private:
	/** Helper variable to fill the bar */
	float Time;

	FTimerHandle CooldownTimer;

	/** Variable that determines the smoothness of filling the bar */
	const float TimerInterval = 0.1f;
};
