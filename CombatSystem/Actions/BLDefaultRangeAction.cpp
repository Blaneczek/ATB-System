// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLDefaultRangeAction.h"
#include "Characters/BLCombatCharacter.h"
#include "PaperZDAnimInstance.h"
#include "PaperZDAnimationComponent.h"
#include "Kismet/GameplayStatics.h"

void UBLDefaultRangeAction::ActivateAction(ABLCombatCharacter* Owner)
{
	if (Owner && ActionAnim && ActionSound)
	{
		Owner->SetCurrentME(FMath::Clamp((Owner->GetCurrentME() - MECost), 0.f, Owner->GetMaxME()));
		UGameplayStatics::PlaySound2D(GetWorld(), ActionSound);
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

	ActionCalculations(Owner, Target);
	FTimerHandle Delay;
	FTimerDelegate DelayDel;
	DelayDel.BindLambda([this]() { OnEndExecution.ExecuteIfBound(); });
	GetWorld()->GetTimerManager().SetTimer(Delay, DelayDel, 0.5f, false);	
}
