// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLActionsWidget.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
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

void UBLActionsWidget::SetActionsData(const FCombatCharData& BaseData, const FCombatActions& CombatActions)
{	
	AttackAction->AddActions(CombatActions.AttackActions);
	DefendAction->AddActions(CombatActions.DefendActions);

	!CombatActions.CrystalActions.IsEmpty() ? CrystalAction->AddActions(CombatActions.CrystalActions) : BTCrystal->SetIsEnabled(false);

	!CombatActions.SpecialActions.IsEmpty() ? SpecialAction->AddActions(CombatActions.SpecialActions) : BTSpecial->SetIsEnabled(false);

	ItemAction->AddActions(CombatActions.ItemActions);

	SpecialActionsName->SetText(BaseData.SpecialActionsName);
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
	ItemAction->OnAction.BindUObject(this, &UBLActionsWidget::ChosenAction);
}

void UBLActionsWidget::OnBTAttackClicked()
{
	ResetButton();
	ResetAction();
	ClickedButton = BTAttack;
	ClickedAction = AttackAction;
	ClickedButton->SetIsEnabled(false);
	AttackAction->ShowDescription(0);
	ActionTypeSwitcher->SetActiveWidget(AttackAction);
	AttackAction->OnAction.ExecuteIfBound(FCombatActionData(ECombatActionType::ATTACK, ECombatActionFlow::NONE, 0));
}

void UBLActionsWidget::OnBTDefendClicked()
{
	ResetButton();
	ResetAction();
	ClickedButton = BTDefend;
	ClickedAction = DefendAction;
	ClickedButton->SetIsEnabled(false);
	DefendAction->ShowDescription(0);
	ActionTypeSwitcher->SetActiveWidget(DefendAction);
	DefendAction->OnAction.ExecuteIfBound(FCombatActionData(ECombatActionType::DEFEND, ECombatActionFlow::NONE, 0));
}

void UBLActionsWidget::OnBTCrystalClicked()
{
	ResetButton();
	ResetAction();
	ClickedButton = BTCrystal;
	ClickedAction = CrystalAction;
	ClickedButton->SetIsEnabled(false);
	ActionTypeSwitcher->SetActiveWidget(CrystalAction);
}

void UBLActionsWidget::OnBTSpecialSkillClicked()
{
	ResetButton();
	ResetAction();
	ClickedButton = BTSpecial;
	ClickedAction = SpecialAction;
	ClickedButton->SetIsEnabled(false);
	ActionTypeSwitcher->SetActiveWidget(SpecialAction);
}

void UBLActionsWidget::OnBTItemClicked()
{
	ResetButton();
	ResetAction();
	ClickedButton = BTItem;
	ClickedAction = ItemAction;
	ClickedButton->SetIsEnabled(false);
	ActionTypeSwitcher->SetActiveWidget(ItemAction);
}

void UBLActionsWidget::OnBTRunClicked()
{

}

void UBLActionsWidget::ResetButton()
{
	if (ClickedButton)
	{
		ClickedButton->SetIsEnabled(true);
		ClickedButton = nullptr;
	}
}

void UBLActionsWidget::ResetAction()
{
	if (ClickedAction)
	{
		OnResetActionType.ExecuteIfBound();
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

void UBLActionsWidget::ChosenAction(const FCombatActionData& ActionData)
{
	OnChosenAction.ExecuteIfBound(ActionData);
}

