// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLDefaultRangeAction.h"
#include "Characters/BLCombatCharacter.h"
#include "Characters/BLActionComponent.h"
#include "PaperZDAnimInstance.h"
#include "PaperZDAnimationComponent.h"
#include "Kismet/GameplayStatics.h"

void UBLDefaultRangeAction::ActivateAction(UBLActionComponent* OwnerComponent)
{
	if (!OwnerComponent || !OwnerChar)
	{
		OnEndExecution.ExecuteIfBound();
		return;
	}

	if (ActionAnim)
	{	
		OwnerChar->GetAnimationComponent()->GetAnimInstance()->PlayAnimationOverride(ActionAnim);
	}

	OwnerChar->SetCurrentME(FMath::Clamp((OwnerChar->GetCurrentME() - MECost), 0.f, OwnerChar->GetMaxME()));
	OwnerComponent->DefaultRangeAction(ProjectileClass, ProjectileSprite);
}

void UBLDefaultRangeAction::ExecuteAction(ABLCombatSlot* Target)
{
	if (!Target)
	{
		OnEndExecution.ExecuteIfBound();
		return;
	}

	ActionCalculations(Target, CombatManager);
	FTimerHandle Delay;
	FTimerDelegate DelayDel;
	DelayDel.BindWeakLambda(this, [this]() { OnEndExecution.ExecuteIfBound(); });
	GetWorld()->GetTimerManager().SetTimer(Delay, DelayDel, 0.5f, false);	
}
