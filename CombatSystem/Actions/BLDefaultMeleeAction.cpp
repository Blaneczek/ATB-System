// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLDefaultMeleeAction.h"
#include "CombatComponents/BLActionComponent.h"
#include "Characters/BLCombatCharacter.h"
#include "PaperZDAnimInstance.h"
#include "PaperZDAnimationComponent.h"
#include "Kismet/GameplayStatics.h"

void UBLDefaultMeleeAction::ActivateAction(UBLActionComponent* OwnerComponent)
{
	if (OwnerComponent && OwnerChar)
	{
		OwnerComponent->DefaultMeleeAction();

		OwnerChar->SetCurrentME(FMath::Clamp((OwnerChar->GetCurrentME() - MECost), 0.f, OwnerChar->GetMaxME()));
	}
}

void UBLDefaultMeleeAction::ExecuteAction(const TArray<ABLCombatSlot*>& Targets)
{
	if (!OwnerChar || !Targets.IsValidIndex(0) || !Targets[0])
	{
		OnEndExecution.ExecuteIfBound();
		return;
	}

	if (ActionAnim)
	{
		FZDOnAnimationOverrideEndSignature EndAnimDel;
		EndAnimDel.BindWeakLambda(this, [this](bool bResult) { OnEndExecution.ExecuteIfBound(); });
		OwnerChar->GetAnimationComponent()->GetAnimInstance()->PlayAnimationOverride(ActionAnim, "DefaultSlot", 1.f, 0.0f, EndAnimDel);
		ActionCalculations(Targets[0], CombatManager);
	}
	else
	{
		ActionCalculations(Targets[0], CombatManager);
		OnEndExecution.ExecuteIfBound();
	}
}

