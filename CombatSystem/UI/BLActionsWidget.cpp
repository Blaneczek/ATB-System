// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLActionsWidget.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Actions/BLAttackActionWidget.h"
#include "Actions/BLDefendActionWidget.h"

void UBLActionsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ClickedButton = nullptr;
	ClickedAction = nullptr;

	BindButtons();
	BindActions();
}

void UBLActionsWidget::SetActionsData(const FAttackActionData& AttackData, const FDefendActionData& DefendData)
{	
	AttackAction->SetActionData(AttackData);
	DefendAction->SetActionData(DefendData);
}

void UBLActionsWidget::BindButtons()
{
	BTAttack->OnClicked.AddDynamic(this, &UBLActionsWidget::OnBTAttackClicked);
	BTDefend->OnClicked.AddDynamic(this, &UBLActionsWidget::OnBTDefendClicked);
	BTCrystal->OnClicked.AddDynamic(this, &UBLActionsWidget::OnBTCrystalClicked);
	BTSpecialSkill->OnClicked.AddDynamic(this, &UBLActionsWidget::OnBTSpecialSkillClicked);
	BTItem->OnClicked.AddDynamic(this, &UBLActionsWidget::OnBTItemClicked);
	BTRun->OnClicked.AddDynamic(this, &UBLActionsWidget::OnBTRunClicked);
}

void UBLActionsWidget::BindActions()
{
	AttackAction->OnAction.BindUObject(this, &UBLActionsWidget::ChosenAction);
	DefendAction->OnAction.BindUObject(this, &UBLActionsWidget::ChosenAction);
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

}

void UBLActionsWidget::OnBTSpecialSkillClicked()
{

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

void UBLActionsWidget::ChosenAction(ECombatActionType Action, int32 ActionIndex)
{
	OnChosenAction.ExecuteIfBound(Action, ActionIndex);
}
