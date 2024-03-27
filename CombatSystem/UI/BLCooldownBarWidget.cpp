// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLCooldownBarWidget.h"
#include "Components/ProgressBar.h"

void UBLCooldownBarWidget::StartCooldown(float Cooldown)
{
	Time = 0.f;
	FTimerDelegate CooldownDel;
	CooldownDel.BindUObject(this, &UBLCooldownBarWidget::UpdateBar, Cooldown);
	GetWorld()->GetTimerManager().SetTimer(CooldownTimer, CooldownDel, TimerInterval, true);
}

void UBLCooldownBarWidget::ResetCooldown()
{
	CooldownBar->SetPercent(0.f);
}

void UBLCooldownBarWidget::PauseCooldown()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(CooldownTimer))
	{
		GetWorld()->GetTimerManager().PauseTimer(CooldownTimer);
	}
}

void UBLCooldownBarWidget::UnPauseCooldown()
{
	if (GetWorld()->GetTimerManager().IsTimerPaused(CooldownTimer))
	{
		GetWorld()->GetTimerManager().UnPauseTimer(CooldownTimer);
	}
}

void UBLCooldownBarWidget::UpdateBar(float Cooldown)
{
	Time += TimerInterval;
	CooldownBar->SetPercent(Time / Cooldown);

	if (Time >= Cooldown && CooldownTimer.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(CooldownTimer);
		OnCooldownEnded.ExecuteIfBound();  
	}
}
