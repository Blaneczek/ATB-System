// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLMultipleInPlace.h"
#include "PaperZDAnimInstance.h"
#include "PaperZDAnimationComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/BLCombatCharacter.h"
#include "BLCombatSlot.h"

void UBLMultipleInPlace::ActivateAction(ABLCombatCharacter* Owner, const TArray<ABLCombatSlot*>& Targets)
{
	if (!Owner)
	{
		OnEndExecution.ExecuteIfBound();
		return;
	}

	if (ActionAnim)
	{
		FZDOnAnimationOverrideEndSignature EndAnimDel;
		EndAnimDel.BindWeakLambda(this, [this, Owner, Targets](bool bResult) { Owner->SummonAction(Targets); });
		Owner->GetAnimationComponent()->GetAnimInstance()->PlayAnimationOverride(ActionAnim, "DefaultSlot", 1.f, 0.0f, EndAnimDel);
	}
	else
	{
		Owner->SummonAction(Targets);
	}
}

void UBLMultipleInPlace::ExecuteAction(ABLCombatCharacter* Owner, const TArray<ABLCombatSlot*>& Targets)
{
	if (Targets.IsEmpty() || !Owner)
	{
		OnEndExecution.ExecuteIfBound();
		return;
	}


	ActionCalculationsMultiAtOnce(Owner, Targets, CombatManager);
	OnEndExecution.ExecuteIfBound();
}
