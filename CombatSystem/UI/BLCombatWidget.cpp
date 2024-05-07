// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLCombatWidget.h"
#include "BLEnemiesWidget.h"
#include "BLHeroesWidget.h"
#include "BLActionsWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Border.h"
#include "Actions/BLAction.h"
#include "Components/TextBlock.h"

void UBLCombatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ActionsSwitcher->SetActiveWidget(NoneActions);
	NotEnoughME->SetVisibility(ESlateVisibility::Hidden);
	DisplayWindow->SetVisibility(ESlateVisibility::Hidden);
	RunAwayDisplay->SetVisibility(ESlateVisibility::Hidden);
	ActionTextDisplay->SetVisibility(ESlateVisibility::Hidden);
}

void UBLCombatWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Actions.Add(Actions_0);
	Actions.Add(Actions_1);
	Actions.Add(Actions_2);
	Actions.Add(Actions_3);
	Actions.Add(Actions_4);
}

void UBLCombatWidget::AddHero(int32 SlotIndex, const FCombatCharData& BaseData, bool bSneakAttack)
{
	Heroes->AddHero(SlotIndex, BaseData.Name, BaseData.MaxHP, BaseData.CurrentHP, BaseData.MaxME);
	Heroes->OnHeroClicked.BindUObject(this, &UBLCombatWidget::HeroClicked);

	// If sneak attack, the cooldown bar will fill up immediately.
	const float Cooldown = bSneakAttack ? 0.1f : BaseData.Cooldown;

	// Cooldown bar will start after 1 sek.
	FTimerDelegate CooldownDel;
	CooldownDel.BindUObject(this, &UBLCombatWidget::StartHeroCooldownBar, SlotIndex, Cooldown);
	FTimerHandle CooldownTimer;
	GetWorld()->GetTimerManager().SetTimer(CooldownTimer, CooldownDel, 1.f, false);
}

void UBLCombatWidget::AddEnemy(int32 SlotIndex, const FString& EnemyName, int32 Level, float Cooldown)
{
	Enemies->AddEnemy(SlotIndex, EnemyName, Level);

	// Cooldown bar will start after 1 sek.
	FTimerDelegate CooldownDel;
	CooldownDel.BindUObject(this, &UBLCombatWidget::StartEnemyCooldownBar, SlotIndex, Cooldown);
	FTimerHandle CooldownTimer;
	GetWorld()->GetTimerManager().SetTimer(CooldownTimer, CooldownDel, 1.f, false);
}

void UBLCombatWidget::RemoveEnemy(int32 SlotIndex)
{
	Enemies->RemoveEnemy(SlotIndex);
}

void UBLCombatWidget::AddHeroActions(int32 SlotIndex, const FCombatCharData& BaseData, const FCombatActions& CombatActions, bool bCanRunAway)
{
	if (Actions.IsValidIndex(SlotIndex) && Actions[SlotIndex])
	{
		Actions[SlotIndex]->OnResetActionType.BindWeakLambda(this, [this]() { OnResetCurrentActionType.ExecuteIfBound(); });
		Actions[SlotIndex]->SetActionsData(BaseData, CombatActions, bCanRunAway);
		Actions[SlotIndex]->OnChosenAction.BindUObject(this, &UBLCombatWidget::ChosenAction);
	}
}

void UBLCombatWidget::SetCurrentHero(int32 SlotIndex)
{
	Heroes->HighlightsHero(SlotIndex, true);
	if (Actions.IsValidIndex(SlotIndex) && Actions[SlotIndex])
	{
		ActionsSwitcher->SetActiveWidget(Actions[SlotIndex]);
	}
}

void UBLCombatWidget::ResetCurrentHero(int32 SlotIndex)
{
	Heroes->HighlightsHero(SlotIndex, false);
	if (Actions.IsValidIndex(SlotIndex) && Actions[SlotIndex])
	{
		Actions[SlotIndex]->ResetWidget();
	}
	ActionsSwitcher->SetActiveWidget(NoneActions);
}

void UBLCombatWidget::HeroDied(int32 SlotIndex, bool bHighlightedHero)
{
	Heroes->HeroDied(SlotIndex);
	if (bHighlightedHero)
	{
		if (Actions.IsValidIndex(SlotIndex) && Actions[SlotIndex])
		{
			Actions[SlotIndex]->ResetWidget();
		}
		ActionsSwitcher->SetActiveWidget(NoneActions);
	}
}

void UBLCombatWidget::EnemyDied(int32 SlotIndex)
{
	Enemies->EnemyDied(SlotIndex);
}

void UBLCombatWidget::PauseCooldownBars(bool bNewPause)
{
	Heroes->PauseAllCooldownBars(bNewPause);
	Enemies->PauseAllCooldownBars(bNewPause);
}

void UBLCombatWidget::StartHeroCooldownBar(int32 SlotIndex, float Cooldown)
{
	Heroes->StartHeroCooldownBar(SlotIndex, Cooldown);
}

void UBLCombatWidget::ResetHeroCooldownBar(int32 SlotIndex)
{
	Heroes->ResetHeroCooldownBar(SlotIndex);
}

void UBLCombatWidget::StartEnemyCooldownBar(int32 SlotIndex, float Cooldown)
{
	Enemies->StartEnemyCooldownBar(SlotIndex, Cooldown);
}

void UBLCombatWidget::ResetEnemyCooldownBar(int32 SlotIndex)
{
	Enemies->ResetEnemyCooldownBar(SlotIndex);
}

void UBLCombatWidget::UpdateHeroHealth(int32 SlotIndex, float MaxHP, float CurrentHP)
{
	Heroes->UpdateHeroHP(SlotIndex, MaxHP, CurrentHP);
}

void UBLCombatWidget::UpdateHeroMagicEnergy(int32 SlotIndex, float MaxME, float CurrentME)
{
	Heroes->UpdateHeroME(SlotIndex, MaxME, CurrentME);
}

void UBLCombatWidget::HideActions()
{
	ActionsSwitcher->SetActiveWidget(NoneActions);
}

void UBLCombatWidget::ActivateNotEnoughME()
{
	if (NotEnoughME->GetVisibility() == ESlateVisibility::Hidden)
	{
		NotEnoughME->SetVisibility(ESlateVisibility::Visible);
		FTimerDelegate DelayDel;
		DelayDel.BindWeakLambda(this, [this]() { NotEnoughME->SetVisibility(ESlateVisibility::Hidden); });
		FTimerHandle DelayTimer;
		GetWorld()->GetTimerManager().SetTimer(DelayTimer, DelayDel, 1.5f, false);
	}
}

void UBLCombatWidget::ShowWindowText(const FText& InText, float Time)
{
	WindowText->SetText(InText);
	if (Time > 0.f)
	{
		FTimerHandle DelayTimer;
		GetWorld()->GetTimerManager().SetTimer(DelayTimer, this, &UBLCombatWidget::HideWindowText, Time, false);
	}

	DisplayWindow->SetVisibility(ESlateVisibility::Visible);
}

void UBLCombatWidget::HideWindowText()
{
	DisplayWindow->SetVisibility(ESlateVisibility::Hidden);
}

void UBLCombatWidget::ShowRunAwayText(bool bSuccessful)
{
	if (bSuccessful)
	{
		GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
		RunAwayDisplay->SetText(FText::FromString("SUCCESSFUL ESCAPE"));
		RunAwayDisplay->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		RunAwayDisplay->SetText(FText::FromString("FAILED TO ESCAPE"));
		RunAwayDisplay->SetVisibility(ESlateVisibility::Visible);
		FTimerDelegate DelayDel;
		DelayDel.BindWeakLambda(this, [this]() {RunAwayDisplay->SetVisibility(ESlateVisibility::Hidden); });
		FTimerHandle DelayTimer;
		GetWorld()->GetTimerManager().SetTimer(DelayTimer, DelayDel, 2.f, false);
	}
}

void UBLCombatWidget::ShowActionTextDisplay(bool bNewShow)
{
	bNewShow ? ActionTextDisplay->SetVisibility(ESlateVisibility::Visible) : ActionTextDisplay->SetVisibility(ESlateVisibility::Hidden);
}

void UBLCombatWidget::ChosenAction(const FCombatActionData& ActionData)
{
	OnActionChosen.ExecuteIfBound(ActionData);
}

void UBLCombatWidget::HeroClicked(int32 HeroIndex)
{
	OnHeroSelected.ExecuteIfBound(HeroIndex);
}
