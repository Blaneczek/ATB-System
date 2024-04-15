// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLColumnMeleeAction.h"
#include "Characters/BLCombatCharacter.h"
#include "PaperZDAnimInstance.h"
#include "PaperZDAnimationComponent.h"
#include "Kismet/GameplayStatics.h"

void UBLColumnMeleeAction::ActivateAction(ABLCombatCharacter* Owner)
{
	if (Owner)
	{
		Owner->ColumnMeleeAction();
		Owner->SetCurrentME(FMath::Clamp((Owner->GetCurrentME() - MECost), 0.f, Owner->GetMaxME()));
	}
}

void UBLColumnMeleeAction::ExecuteAction(ABLCombatCharacter* Owner, ABLCombatCharacter* Target)
{
	if (!Owner || !Target && bEndAction)
	{
		OnEndExecution.ExecuteIfBound();
	}

	if (ActionAnim && ActionSound)
	{
		if (bEndAction)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ActionSound);
			FZDOnAnimationOverrideEndSignature EndAnimDel;
			EndAnimDel.BindLambda([this](bool bResult) { OnEndExecution.ExecuteIfBound(); });
			Owner->GetAnimationComponent()->GetAnimInstance()->PlayAnimationOverride(ActionAnim, "DefaultSlot", 1.f, 0.0f, EndAnimDel);
		}		
		ActionCalculations(Owner, Target, CombatManager);
	}

	bEndAction = false;
}
