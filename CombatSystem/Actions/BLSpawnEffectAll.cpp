// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLSpawnEffectAll.h"
#include "PaperZDAnimInstance.h"
#include "PaperZDAnimationComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/BLCombatCharacter.h"

void UBLSpawnEffectAll::ActivateAction(ABLCombatCharacter* Owner, const TArray<ABLCombatSlot*>& Targets)
{
	if (!Owner)
	{
		OnEndExecution.ExecuteIfBound();
		return;
	}

	if (ActionAnim)
	{
		FZDOnAnimationOverrideEndSignature EndAnimDel;
		EndAnimDel.BindWeakLambda(this, [this, Owner, Targets](bool bResult) { Owner->SpawnEffectsAllAction(Targets, EffectClass, Effect); });
		Owner->GetAnimationComponent()->GetAnimInstance()->PlayAnimationOverride(ActionAnim, "DefaultSlot", 1.f, 0.0f, EndAnimDel);
	}
	else
	{
		Owner->SpawnEffectsAllAction(Targets, EffectClass, Effect);
	}
}

void UBLSpawnEffectAll::ExecuteAction(ABLCombatCharacter* Owner, const TArray<ABLCombatSlot*>& Targets)
{
	if (Targets.IsEmpty() || !Owner)
	{
		OnEndExecution.ExecuteIfBound();
		return;
	}

	ActionCalculationsMultiAtOnce(Owner, Targets, CombatManager);
	OnEndExecution.ExecuteIfBound();
}
