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

void UBLActionsWidget::SetActionsData(const FCombatCharData& BaseData, const FCombatActions& CombatActions, bool bCanRunAway)
{	
	!CombatActions.AttackActions.IsEmpty() ? AttackAction->AddActions(CombatActions.AttackActions) : DisableButton(BTAttack);

	!CombatActions.DefendAction.IsNull() ? DefendAction->AddActions(CombatActions.DefendAction) : DisableButton(BTDefend);

	!CombatActions.CrystalActions.IsEmpty() ? CrystalAction->AddActions(CombatActions.CrystalActions) : DisableButton(BTCrystal);

	!CombatActions.SpecialActions.IsEmpty() ? SpecialAction->AddActions(CombatActions.SpecialActions) : DisableButton(BTSpecial);

	ItemAction->AddActions(CombatActions.ItemActions);

	if (!bCanRunAway)
	{
		DisableButton(BTRun);
	}
	else
	{
		!CombatActions.RunAwayAction.IsNull() ? RunAwayAction->AddActions(CombatActions.RunAwayAction) : DisableButton(BTRun);
	}

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
	RunAwayAction->OnAction.BindUObject(this, &UBLActionsWidget::ChosenAction);
}

void UBLActionsWidget::DisableButton(UButton* Button)
{
	FButtonStyle Style = Button->GetStyle();
	Style.Disabled.TintColor = FLinearColor(0.3f, 0.3f, 0.3f, 1.f);
	Style.Disabled.OutlineSettings.Width = 0.f;

	Button->SetStyle(Style);
	Button->SetIsEnabled(false);
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
	ResetButton();
	ResetAction();
	ClickedButton = BTRun;
	ClickedAction = RunAwayAction;
	ClickedButton->SetIsEnabled(false);
	ActionTypeSwitcher->SetActiveWidget(RunAwayAction);
	RunAwayAction->OnAction.ExecuteIfBound(FCombatActionData(ECombatActionType::RUN_AWAY, ECombatActionFlow::NONE, 0));
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

