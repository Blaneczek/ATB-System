// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLBounceRangeAction.h"
#include "Characters/BLCombatCharacter.h"
#include "Characters/BLActionComponent.h"
#include "PaperZDAnimInstance.h"
#include "PaperZDAnimationComponent.h"
#include "Kismet/GameplayStatics.h"

void UBLBounceRangeAction::ActivateAction(UBLActionComponent* OwnerComponent)
{
	if (!OwnerComponent || !OwnerChar)
	{
		OnEndExecution.ExecuteIfBound();
		return;
	}

	OwnerChar->SetCurrentME(FMath::Clamp((OwnerChar->GetCurrentME() - MECost), 0.f, OwnerChar->GetMaxME()));

	if (ActionAnim)
	{		
		FZDOnAnimationOverrideEndSignature EndAnimDel;
		EndAnimDel.BindWeakLambda(this, [this, OwnerComponent](bool bResult) { OwnerComponent->BounceRangeAction(ProjectileClass, ProjectileSprite); });
		OwnerChar->GetAnimationComponent()->GetAnimInstance()->PlayAnimationOverride(ActionAnim, "DefaultSlot", 1.f, 0.0f, EndAnimDel);
	}
	else
	{
		OwnerComponent->BounceRangeAction(ProjectileClass, ProjectileSprite);
	}
}

void UBLBounceRangeAction::ExecuteAction(ABLCombatSlot* Target)
{
	if (!Target)
	{
		OnEndExecution.ExecuteIfBound();
		return;
	}

	ActionCalculations(Target, CombatManager);
	OnEndExecution.ExecuteIfBound();
}
