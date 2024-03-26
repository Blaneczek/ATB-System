// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLDefaultMeleeAction.h"
#include "Characters/BLCombatCharacter.h"
#include "PaperZDAnimInstance.h"
#include "PaperZDAnimationComponent.h"

void UBLDefaultMeleeAction::ActivateAction(ABLCombatCharacter* Owner)
{
	if (Owner)
	{
		Owner->DefaultMeleeAction();
	}
}

void UBLDefaultMeleeAction::ExecuteAction(ABLCombatCharacter* Owner, ABLCombatCharacter* Target)
{
	if (!Owner || !Target)
	{
		OnEndExecution.ExecuteIfBound();
	}

	Owner->SetCurrentME(FMath::Clamp((Owner->GetCurrentME() - MECost), 0.f, Owner->GetMaxME()));

	if (ActionAnim)
	{
		FZDOnAnimationOverrideEndSignature EndAnimDel;
		EndAnimDel.BindLambda([this](bool bResult) { OnEndExecution.ExecuteIfBound(); });
		Owner->GetAnimationComponent()->GetAnimInstance()->PlayAnimationOverride(ActionAnim, "DefaultSlot", 1.f, 0.0f, EndAnimDel);
		ActionCalculations(Owner, Target);
	}
}

