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

void UBLCombatWidget::AddHero(int32 SlotIndex, const FCombatCharData& BaseData)
{
	Heroes->AddHero(SlotIndex, BaseData.Name, BaseData.MaxHP, BaseData.MaxME);
	Heroes->OnHeroClicked.BindUObject(this, &UBLCombatWidget::HeroClicked);
	//Cooldown bar will start after 1 sek
	FTimerDelegate CooldownDel;
	CooldownDel.BindUObject(this, &UBLCombatWidget::StartHeroCooldownBar, SlotIndex, BaseData.Cooldown);
	FTimerHandle CooldownTimer;
	GetWorld()->GetTimerManager().SetTimer(CooldownTimer, CooldownDel, 1.f, false);
}

void UBLCombatWidget::AddEnemy(int32 SlotIndex, const FString& EnemyName)
{
	Enemies->AddEnemy(SlotIndex, EnemyName);
}

void UBLCombatWidget::AddHeroActions(int32 SlotIndex, const FCombatCharData& BaseData, const TArray<TSoftClassPtr<UBLAction>>& InAttackActions, const TArray<TSoftClassPtr<UBLAction>>& InDefendActions, const TMap<ECrystalColor, FCrystalSkills>& InCrystalActions, const TArray<TSoftClassPtr<UBLAction>>& InSpecialActions)
{
	if (Actions.IsValidIndex(SlotIndex) && Actions[SlotIndex])
	{
		Actions[SlotIndex]->SetActionsData(BaseData, InAttackActions, InDefendActions, InCrystalActions, InSpecialActions);
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

void UBLCombatWidget::HeroDied(int32 SlotIndex)
{
	Heroes->HeroDied(SlotIndex);
	if (Actions.IsValidIndex(SlotIndex) && Actions[SlotIndex])
	{
		Actions[SlotIndex]->ResetWidget();
	}
	ActionsSwitcher->SetActiveWidget(NoneActions);
}

void UBLCombatWidget::EnemyDied(int32 SlotIndex)
{
	Enemies->EnemyDied(SlotIndex);
}

void UBLCombatWidget::PauseCooldownBars(bool bNewPause)
{
	Heroes->PauseAllCooldownBars(bNewPause);
}

void UBLCombatWidget::StartHeroCooldownBar(int32 SlotIndex, float Cooldown)
{
	Heroes->StartHeroCooldownBar(SlotIndex, Cooldown);
}

void UBLCombatWidget::ResetHeroCooldownBar(int32 SlotIndex)
{
	Heroes->ResetHeroCooldownBar(SlotIndex);
}

void UBLCombatWidget::SetIsActionVisibility(ESlateVisibility InVisibility)
{
	//
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
		DelayDel.BindLambda([this]() { NotEnoughME->SetVisibility(ESlateVisibility::Hidden); });
		FTimerHandle DelayTimer;
		GetWorld()->GetTimerManager().SetTimer(DelayTimer, DelayDel, 1.5f, false);
	}
}

void UBLCombatWidget::ChosenAction(ECombatActionType Action, int32 ActionIndex, ECrystalColor CrystalColor, float ActionMECost, int32 TargetsNum, UObject* ActionEntry)
{
	if (ActionEntry)
	{
		UE_LOG(LogTemp, Warning, TEXT("combat widget jest"));
	}
	OnActionChosen.ExecuteIfBound(Action, ActionIndex, CrystalColor, ActionMECost, TargetsNum, ActionEntry);
}

void UBLCombatWidget::HeroClicked(int32 HeroIndex)
{
	OnHeroSelected.ExecuteIfBound(HeroIndex);
}
