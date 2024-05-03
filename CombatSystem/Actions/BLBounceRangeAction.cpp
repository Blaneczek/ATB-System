// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLBounceRangeAction.h"
#include "Characters/BLCombatCharacter.h"
#include "PaperZDAnimInstance.h"
#include "PaperZDAnimationComponent.h"
#include "Kismet/GameplayStatics.h"

void UBLBounceRangeAction::ActivateAction(ABLCombatCharacter* Owner)
{
	if (!Owner)
	{
		OnEndExecution.ExecuteIfBound();
		return;
	}
	Owner->SetCurrentME(FMath::Clamp((Owner->GetCurrentME() - MECost), 0.f, Owner->GetMaxME()));

	if (ActionAnim)
	{		
		FZDOnAnimationOverrideEndSignature EndAnimDel;
		EndAnimDel.BindWeakLambda(this, [this, Owner](bool bResult) { Owner->BounceRangeAction(ProjectileClass, ProjectileSprite); });
		Owner->GetAnimationComponent()->GetAnimInstance()->PlayAnimationOverride(ActionAnim, "DefaultSlot", 1.f, 0.0f, EndAnimDel);
	}
	else
	{
		Owner->BounceRangeAction(ProjectileClass, ProjectileSprite);
	}
}

void UBLBounceRangeAction::ExecuteAction(ABLCombatCharacter* Owner, ABLCombatCharacter* Target)
{
	if (!Owner || !Target)
	{
		OnEndExecution.ExecuteIfBound();
		return;
	}

	ActionCalculations(Owner, Target, CombatManager);
	OnEndExecution.ExecuteIfBound();
}
