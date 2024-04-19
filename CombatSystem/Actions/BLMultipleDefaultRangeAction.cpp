// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLMultipleDefaultRangeAction.h"
#include "Characters/BLCombatCharacter.h"
#include "PaperZDAnimInstance.h"
#include "PaperZDAnimationComponent.h"
#include "Kismet/GameplayStatics.h"

void UBLMultipleDefaultRangeAction::ActivateAction(ABLCombatCharacter* Owner)
{
	if (Owner && ActionAnim)
	{
		Owner->SetCurrentME(FMath::Clamp((Owner->GetCurrentME() - MECost), 0.f, Owner->GetMaxME()));
		FZDOnAnimationOverrideEndSignature EndAnimDel;
		EndAnimDel.BindLambda([this, Owner](bool bResult) { Owner->MultipleDefaultRangeAction(ProjectileClass, ProjectileSprite); });
		Owner->GetAnimationComponent()->GetAnimInstance()->PlayAnimationOverride(ActionAnim, "DefaultSlot", 1.f, 0.0f, EndAnimDel);
	}
}

void UBLMultipleDefaultRangeAction::ExecuteAction(ABLCombatCharacter* Owner, ABLCombatCharacter* Target)
{
	if (!Owner || !Target)
	{
		OnEndExecution.ExecuteIfBound();
	}

	ActionCalculations(Owner, Target, CombatManager);
	FTimerHandle Delay;
	FTimerDelegate DelayDel;
	DelayDel.BindLambda([this]() { OnEndExecution.ExecuteIfBound(); });
	GetWorld()->GetTimerManager().SetTimer(Delay, DelayDel, 0.1f, false);
}
