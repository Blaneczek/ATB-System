// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLDefaultRangeAction.h"
#include "Characters/BLCombatCharacter.h"
#include "PaperZDAnimInstance.h"
#include "PaperZDAnimationComponent.h"

void UBLDefaultRangeAction::ActivateAction(ABLCombatCharacter* Owner)
{
	if (Owner && ActionAnim)
	{
		FZDOnAnimationOverrideEndSignature EndAnimDel;
		EndAnimDel.BindLambda([this, Owner](bool bResult) { Owner->DefaultRangeAction(ProjectileClass, ProjectileSprite); });
		Owner->GetAnimationComponent()->GetAnimInstance()->PlayAnimationOverride(ActionAnim, "DefaultSlot", 1.f, 0.0f, EndAnimDel);
	}
}

void UBLDefaultRangeAction::ExecuteAction(ABLCombatCharacter* Owner, ABLCombatCharacter* Target)
{
	if (!Owner || !Target)
	{
		OnEndExecution.ExecuteIfBound();
	}

	Target->HandleHitByAction(Owner->BaseData.AttackDMG, Element);
	FTimerHandle Delay;
	FTimerDelegate DelayDel;
	DelayDel.BindLambda([this]() { OnEndExecution.ExecuteIfBound(); });
	GetWorld()->GetTimerManager().SetTimer(Delay, DelayDel, 1.5f, false);	
}
