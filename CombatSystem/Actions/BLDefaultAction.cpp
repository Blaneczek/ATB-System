// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLDefaultAction.h"
#include "Characters/BLCombatCharacter.h"
#include "PaperZDAnimInstance.h"
#include "PaperZDAnimationComponent.h"
#include "Kismet/GameplayStatics.h"

void UBLDefaultAction::ActivateAction(ABLCombatCharacter* Owner)
{
	if (Owner)
	{
		Owner->DefaultAction();	
		Owner->SetCurrentME(FMath::Clamp((Owner->GetCurrentME() - MECost), 0.f, Owner->GetMaxME()));
	}		
}

void UBLDefaultAction::ExecuteAction(ABLCombatCharacter* Owner, ABLCombatCharacter* Target)
{
	if (!Owner)
	{
		OnEndExecution.ExecuteIfBound();
	}

	ActionCalculations(Owner, Target);

	if (ActionAnim && ActionSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ActionSound);
		FZDOnAnimationOverrideEndSignature EndAnimDel;
		EndAnimDel.BindLambda([this](bool bResult) { OnEndExecution.ExecuteIfBound(); });
		Owner->GetAnimationComponent()->GetAnimInstance()->PlayAnimationOverride(ActionAnim, "DefaultSlot", 1.f, 0.0f, EndAnimDel);
	}
	else
	{
		OnEndExecution.ExecuteIfBound();
	}
}
