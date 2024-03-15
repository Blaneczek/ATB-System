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
	void StartCooldown(float Cooldown);
	void ResetCooldown();
	void PauseCooldown();
	void UnPauseCooldown();

private:
	void UpdateBar(float Cooldown);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat", meta = (BindWidget))
	TObjectPtr<UProgressBar> CooldownBar;

private:
	float Time;
	FTimerHandle CooldownTimer;

	const float TimerInterval = 0.1f;
};
