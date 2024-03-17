// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLAttackActionWidget.h"
#include "Components/TextBlock.h"

void UBLAttackActionWidget::SetActionData(const FAttackActionData& AttackData)
{
	ActionName->SetText(AttackData.Name);
	Description = AttackData.Description;
}

void UBLAttackActionWidget::OnBTActionClicked()
{
	Super::OnBTActionClicked();

	OnAction.ExecuteIfBound(ECombatActionType::ATTACK, 0);
}
