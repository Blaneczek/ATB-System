// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLSummonAction.h"
#include "PaperZDAnimInstance.h"
#include "PaperZDAnimationComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/BLCombatCharacter.h"
#include "BLCombatSlot.h"

void UBLSummonAction::ActivateAction(ABLCombatCharacter* Owner, const TArray<ABLCombatSlot*>& Targets)
{
	if (!Owner)
	{
		OnEndExecution.ExecuteIfBound();
		return;
	}

	if (ActionAnim)
	{
		FZDOnAnimationOverrideEndSignature EndAnimDel;
		EndAnimDel.BindWeakLambda(this, [this, Owner, Targets](bool bResult) { Owner->SummonAction(Targets); });
		Owner->GetAnimationComponent()->GetAnimInstance()->PlayAnimationOverride(ActionAnim, "DefaultSlot", 1.f, 0.0f, EndAnimDel);
	}
	else
	{
		Owner->SummonAction(Targets);
	}
}

void UBLSummonAction::ExecuteAction(ABLCombatCharacter* Owner, const TArray<ABLCombatSlot*>& Targets)
{
	if (Targets.IsEmpty())
	{
		OnEndExecution.ExecuteIfBound();
		return;
	}

	for (auto& Slot : Targets)
	{
		if (Slot)
		{
			Slot->DestroyCharacter();
			const int32 Random = FMath::RandRange(0, EnemiesToSpawn.Num() - 1);
			Slot->SpawnEnemy(EnemiesToSpawn[Random].BaseData, EnemiesToSpawn[Random].Actions, false);
		}
	}
	OnEndExecution.ExecuteIfBound();
}
