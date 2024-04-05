// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLActionsWidget.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "UI/Actions/BLAttackActionWidget.h"
#include "UI/Actions/BLDefendActionWidget.h"
#include "UI/Actions/BLCrystalActionWidget.h"
#include "Actions/BLAction.h"

void UBLActionsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ClickedButton = nullptr;
	ClickedAction = nullptr;

	BindButtons();
	BindActions();
}

void UBLActionsWidget::SetActionsData(const FCombatCharData& BaseData, const TArray<TSoftClassPtr<UBLAction>>& InAttackActions, const TArray<TSoftClassPtr<UBLAction>>& InDefendActions, const TMap<ECrystalColor, FCrystalSkills>& InCrystalActions, const TArray<TSoftClassPtr<UBLAction>>& InSpecialActions)
{	
	AttackAction->AddActions(InAttackActions);
	DefendAction->AddActions(InDefendActions);
	CrystalAction->AddActions(InCrystalActions);
	SpecialAction->AddActions(InSpecialActions);
}

void UBLActionsWidget::BindButtons()
{
	BTAttack->OnClicked.AddDynamic(this, &UBLActionsWidget::OnBTAttackClicked);
	BTDefend->OnClicked.AddDynamic(this, &UBLActionsWidget::OnBTDefendClicked);
	BTCrystal->OnClicked.AddDynamic(this, &UBLActionsWidget::OnBTCrystalClicked);
	BTSpecial->OnClicked.AddDynamic(this, &UBLActionsWidget::OnBTSpecialSkillClicked);
	BTItem->OnClicked.AddDynamic(this, &UBLActionsWidget::OnBTItemClicked);
	BTRun->OnClicked.AddDynamic(this, &UBLActionsWidget::OnBTRunClicked);
}

void UBLActionsWidget::BindActions()
{
	AttackAction->OnAction.BindUObject(this, &UBLActionsWidget::ChosenAction);
	DefendAction->OnAction.BindUObject(this, &UBLActionsWidget::ChosenAction);
	CrystalAction->OnAction.BindUObject(this, &UBLActionsWidget::ChosenAction);
	SpecialAction->OnAction.BindUObject(this, &UBLActionsWidget::ChosenAction);
	//other actions
}

void UBLActionsWidget::OnBTAttackClicked()
{
	ResetButton();
	ResetAction();
	ClickedButton = BTAttack;
	ClickedAction = AttackAction;
	ClickedButton->SetBackgroundColor(FLinearColor(0.3f, 0.3f, 0.3f, 1.f));
	ActionTypeSwitcher->SetActiveWidget(AttackAction);
}

void UBLActionsWidget::OnBTDefendClicked()
{
	ResetButton();
	ResetAction();
	ClickedButton = BTDefend;
	ClickedAction = DefendAction;
	ClickedButton->SetBackgroundColor(FLinearColor(0.3f, 0.3f, 0.3f, 1.f));
	ActionTypeSwitcher->SetActiveWidget(DefendAction);
}

void UBLActionsWidget::OnBTCrystalClicked()
{
	ResetButton();
	ResetAction();
	ClickedButton = BTCrystal;
	ClickedAction = CrystalAction;
	ClickedButton->SetBackgroundColor(FLinearColor(0.3f, 0.3f, 0.3f, 1.f));
	ActionTypeSwitcher->SetActiveWidget(CrystalAction);
}

void UBLActionsWidget::OnBTSpecialSkillClicked()
{
	ResetButton();
	ResetAction();
	ClickedButton = BTSpecial;
	ClickedAction = SpecialAction;
	ClickedButton->SetBackgroundColor(FLinearColor(0.3f, 0.3f, 0.3f, 1.f));
	ActionTypeSwitcher->SetActiveWidget(SpecialAction);
}

void UBLActionsWidget::OnBTItemClicked()
{

}

void UBLActionsWidget::OnBTRunClicked()
{

}

void UBLActionsWidget::ResetButton()
{
	if (ClickedButton)
	{
		ClickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
		ClickedButton = nullptr;
	}
}

void UBLActionsWidget::ResetAction()
{
	if (ClickedAction)
	{
		ClickedAction->ResetAction();
		ClickedAction = nullptr;
	}
}

void UBLActionsWidget::ResetWidget()
{
	ResetButton();
	ResetAction();
	ActionTypeSwitcher->SetActiveWidget(NoneAction);
}

void UBLActionsWidget::ChosenAction(ECombatActionType Action, int32 ActionIndex, ECrystalColor CrystalColor, float ActionMECost, int32 TargetsNum, UObject* ActionEntry)
{
	if (ActionEntry)
	{
		UE_LOG(LogTemp, Warning, TEXT("actions jest"));
	}
	OnChosenAction.ExecuteIfBound(Action, ActionIndex, CrystalColor, ActionMECost, TargetsNum, ActionEntry);
}
