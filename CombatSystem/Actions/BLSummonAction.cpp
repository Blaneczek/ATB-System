// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLSummonAction.h"
#include "PaperZDAnimInstance.h"
#include "PaperZDAnimationComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/BLCombatCharacter.h"
#include "CombatComponents/BLActionComponent.h"
#include "Core/BLCombatSlot.h"

void UBLSummonAction::ActivateAction(UBLActionComponent* OwnerComponent)
{
	if (!OwnerComponent || !OwnerChar)
	{
		OnEndExecution.ExecuteIfBound();
		return;
	}

	if (ActionAnim)
	{
		FZDOnAnimationOverrideEndSignature EndAnimDel;
		EndAnimDel.BindWeakLambda(this, [this, OwnerComponent](bool bResult) { OwnerComponent->MultipleInPlaceAction(EffectClass); });
		OwnerChar->GetAnimationComponent()->GetAnimInstance()->PlayAnimationOverride(ActionAnim, "DefaultSlot", 1.f, 0.0f, EndAnimDel);
	}
	else
	{
		OwnerComponent->MultipleInPlaceAction(EffectClass);
	}
}

void UBLSummonAction::ExecuteAction(const TArray<ABLCombatSlot*>& Targets)
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
			Slot->SpawnEnemy(EnemiesToSpawn[Random].BaseData, EnemiesToSpawn[Random].Level, EnemiesToSpawn[Random].Actions, false);
			ActionCalculations(Slot, CombatManager);
		}
	}
	OnEndExecution.ExecuteIfBound();
}
