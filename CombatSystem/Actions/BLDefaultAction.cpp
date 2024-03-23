// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLDefaultAction.h"
#include "Characters/BLCombatCharacter.h"
#include "PaperZDAnimInstance.h"
#include "PaperZDAnimationComponent.h"

void UBLDefaultAction::ActivateAction(ABLCombatCharacter* Owner)
{
	if (Owner)
	{
		Owner->DefaultAction();
	}
		
}

void UBLDefaultAction::ExecuteAction(ABLCombatCharacter* Owner, ABLCombatCharacter* Target)
{
	if (!Owner)
	{
		OnEndExecution.ExecuteIfBound();
	}

	Owner->CurrentDefense = Owner->BaseData.BaseDefense * 1.5f;
	Owner->bDefendIdle = true;
	if (ActionAnim)
	{
		FZDOnAnimationOverrideEndSignature EndAnimDel;
		EndAnimDel.BindLambda([this](bool bResult) { OnEndExecution.ExecuteIfBound(); });
		Owner->GetAnimationComponent()->GetAnimInstance()->PlayAnimationOverride(ActionAnim, "DefaultSlot", 1.f, 0.0f, EndAnimDel);
	}
	else
	{
		OnEndExecution.ExecuteIfBound();
	}
}
