// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLMultipleInPlaceAction.h"
#include "PaperZDAnimInstance.h"
#include "PaperZDAnimationComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/BLCombatCharacter.h"
#include "Characters/BLActionComponent.h"

void UBLMultipleInPlaceAction::ActivateAction(UBLActionComponent* OwnerComponent)
{
	if (!OwnerComponent || !OwnerChar)
	{
		OnEndExecution.ExecuteIfBound();
		return;
	}

	if (ActionAnim)
	{
		FZDOnAnimationOverrideEndSignature EndAnimDel;
		EndAnimDel.BindWeakLambda(this, [this, OwnerComponent](bool bResult) { OwnerComponent->MultipleInPlaceAction(EffectClass); });
		OwnerChar->GetAnimationComponent()->GetAnimInstance()->PlayAnimationOverride(ActionAnim, "DefaultSlot", 1.f, 0.0f, EndAnimDel);
	}
	else
	{
		OwnerComponent->MultipleInPlaceAction(EffectClass);
	}
}

void UBLMultipleInPlaceAction::ExecuteAction(const TArray<ABLCombatSlot*>& Targets)
{
	if (Targets.IsEmpty())
	{
		OnEndExecution.ExecuteIfBound();
		return;
	}

	ActionCalculationsMultiAtOnce(Targets, CombatManager);
	OnEndExecution.ExecuteIfBound();
}
