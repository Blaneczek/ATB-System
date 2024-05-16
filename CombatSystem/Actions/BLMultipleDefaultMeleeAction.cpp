// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLMultipleDefaultMeleeAction.h"
#include "Characters/BLCombatCharacter.h"
#include "Core/BLCombatSlot.h"
#include "CombatComponents/BLActionComponent.h"
#include "PaperZDAnimInstance.h"
#include "PaperZDAnimationComponent.h"
#include "Kismet/GameplayStatics.h"

void UBLMultipleDefaultMeleeAction::ActivateAction(UBLActionComponent* OwnerComponent)
{
	if (OwnerComponent && OwnerChar)
	{
		OwnerComponent->MultipleDefaultMeleeAction();
		OwnerChar->SetCurrentME(FMath::Clamp((OwnerChar->GetCurrentME() - MECost), 0.f, OwnerChar->GetMaxME()));
	}
}

void UBLMultipleDefaultMeleeAction::ExecuteAction(ABLCombatSlot* Target)
{
	if (!OwnerChar || !Target || Target->GetCharacter()->IsDead())
	{
		OnEndExecution.ExecuteIfBound();
		return;
	}

	if (ActionAnim)
	{
		FZDOnAnimationOverrideEndSignature EndAnimDel;
		EndAnimDel.BindWeakLambda(this, [this](bool bResult) { OnEndExecution.ExecuteIfBound(); });
		OwnerChar->GetAnimationComponent()->GetAnimInstance()->PlayAnimationOverride(ActionAnim, "DefaultSlot", 1.f, 0.0f, EndAnimDel);
		ActionCalculations(Target, CombatManager);
	}
	else
	{
		ActionCalculations(Target, CombatManager);
		OnEndExecution.ExecuteIfBound();
	}
}
 