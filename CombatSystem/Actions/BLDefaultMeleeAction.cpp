// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLDefaultMeleeAction.h"
#include "Characters/BLCombatCharacter.h"
#include "PaperZDAnimInstance.h"
#include "PaperZDAnimationComponent.h"

void UBLDefaultMeleeAction::ActivateAction(ABLCombatCharacter* Owner)
{
	if (Owner)
	{
		IBLActionsInterface* ActionsInterface = Cast<IBLActionsInterface>(Owner);
		if (ActionsInterface)
		{
			Owner->DefaultMeleeAction();
		}
	}
}

void UBLDefaultMeleeAction::ExecuteAction(ABLCombatCharacter* Owner, ABLCombatCharacter* Target)
{
	if (!Owner || !Target || !ActionAnim)
	{
		OnEndExecution.ExecuteIfBound();
	}

	if (ActionAnim)
	{
		FZDOnAnimationOverrideEndSignature EndAnimDel;
		EndAnimDel.BindLambda([this](bool bResult) { OnEndExecution.ExecuteIfBound(); });
		Owner->GetAnimationComponent()->GetAnimInstance()->PlayAnimationOverride(ActionAnim, "DefaultSlot", 1.f, 0.0f, EndAnimDel);
		Target->HandleHitByAction(Owner->BaseData.AttackDMG, Element);
	}
}

