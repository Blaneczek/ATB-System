// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLDefendActionWidget.h"
#include "Components/TextBlock.h"

void UBLDefendActionWidget::SetActionData(const FDefendActionData& DefendData)
{
	ActionName->SetText(DefendData.Name);
	Description = DefendData.Description;
}

void UBLDefendActionWidget::OnBTActionClicked()
{
	Super::OnBTActionClicked();

	OnAction.ExecuteIfBound(ECombatActionType::DEFEND, 0);
}
