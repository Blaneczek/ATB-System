// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLAttackActionWidget.h"
#include "Components/TextBlock.h"
#include "Actions/BLAction.h"

void UBLAttackActionWidget::SetActionData(const TArray<TSoftClassPtr<UBLAction>>& InAttackActions, float AttackDMG)
{
	if (!InAttackActions.IsValidIndex(0))
	{
		return;
	}

	UBLAction* Action = Cast<UBLAction>(InAttackActions[0].LoadSynchronous()->GetDefaultObject());
	if (Action)
	{
		UE_LOG(LogTemp, Warning, TEXT("data set"));
		ActionName->SetText(Action->Name);
		FFormatNamedArguments Args;
		Args.Add(TEXT("Desc"), Action->Description);
		Args.Add(TEXT("Elem"), UEnum::GetDisplayValueAsText(Action->Element));
		Args.Add(TEXT("DMG"), AttackDMG);
		Description = FText::Format(FText::FromString("{Desc}\rElement: {Elem}\rDMG: {DMG}"), Args);
	}
}

void UBLAttackActionWidget::OnBTActionClicked()
{
	Super::OnBTActionClicked();

	OnAction.ExecuteIfBound(ECombatActionType::ATTACK, 0);
}
