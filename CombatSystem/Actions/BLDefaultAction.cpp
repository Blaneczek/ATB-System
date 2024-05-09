// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLDefaultAction.h"
#include "Characters/BLCombatCharacter.h"
#include "Characters/BLActionComponent.h"
#include "PaperZDAnimInstance.h"
#include "PaperZDAnimationComponent.h"
#include "Kismet/GameplayStatics.h"

void UBLDefaultAction::ActivateAction(UBLActionComponent* OwnerComponent)
{
	if (OwnerComponent && OwnerChar)
	{
		OwnerComponent->DefaultAction();	
		OwnerChar->SetCurrentME(FMath::Clamp((OwnerChar->GetCurrentME() - MECost), 0.f, OwnerChar->GetMaxME()));
	}		
}

void UBLDefaultAction::ExecuteAction(ABLCombatSlot* Target)
{
	// Target is null, action performed on OwnerChar
	if (!OwnerChar)
	{
		OnEndExecution.ExecuteIfBound();
		return;
	}

	ActionCalculations(Target, CombatManager);

	if (ActionAnim)
	{
		FZDOnAnimationOverrideEndSignature EndAnimDel;
		EndAnimDel.BindWeakLambda(this, [this](bool bResult) { OnEndExecution.ExecuteIfBound(); });
		OwnerChar->GetAnimationComponent()->GetAnimInstance()->PlayAnimationOverride(ActionAnim, "DefaultSlot", 1.f, 0.0f, EndAnimDel);
	}
	else
	{
		OnEndExecution.ExecuteIfBound();
	}
}
