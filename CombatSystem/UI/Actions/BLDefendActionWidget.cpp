// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLDefendActionWidget.h"
#include "Components/TextBlock.h"
#include "Actions/BLAction.h"

void UBLDefendActionWidget::SetActionData(const TArray<TSoftClassPtr<UBLAction>>& InDefendActions)
{
	if (!InDefendActions.IsValidIndex(0))
	{
		return;
	}

	UBLAction* Action = Cast<UBLAction>(InDefendActions[0].LoadSynchronous()->GetDefaultObject());
	if (Action)
	{
		ActionName->SetText(Action->Name);
		Description = Action->Description;
	}
}

void UBLDefendActionWidget::OnBTActionClicked()
{
	Super::OnBTActionClicked();

	OnAction.ExecuteIfBound(ECombatActionType::DEFEND, 0);
}
