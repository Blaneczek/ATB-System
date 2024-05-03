// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLCombatEnemy.h"
#include "Actions/BLAction.h"

void ABLCombatEnemy::SneakAttack()
{
	const float DMG = BaseData.MaxHP * 0.1;

	// There is a camera fade at the beginning of the combat so we need a small delay to see DMGDisplay widget.
	FTimerDelegate DelayDel;
	DelayDel.BindUObject(this, &ABLCombatEnemy::TakeSimpleDamage, DMG);
	FTimerHandle DelayTimer;
	GetWorld()->GetTimerManager().SetTimer(DelayTimer, DelayDel, 1.f, false);
}

FCombatActionData ABLCombatEnemy::GetEnemyAction()
{
	if (AttackActions.IsEmpty())
	{
		return FCombatActionData();
	}

	TArray<int32> AvailableActions;

	for (int32 Index = 0; Index < AttackActions.Num(); ++Index)
	{
		if (!ActionsCooldown.Contains(Index))
		{
			AvailableActions.Add(Index);
		}
	}

	if (AvailableActions.IsEmpty())
	{
		return FCombatActionData();
	}

	const int32 RandomIndex = FMath::RandRange(0, AvailableActions.Num() - 1);

	UBLAction* Action = Cast<UBLAction>(AttackActions[AvailableActions[RandomIndex]].LoadSynchronous()->GetDefaultObject());
	if (Action)
	{
		UE_LOG(LogTemp, Warning, TEXT("index: %s"), *FString::FromInt(AvailableActions[RandomIndex]));
		if (Action->TurnsCost > 0)
		{
			ActionsCooldown.Add(AvailableActions[RandomIndex], Action->TurnsCost);
		}	
		return FCombatActionData(ECombatActionType::ATTACK, Action->Flow, AvailableActions[RandomIndex], ECrystalColor::NONE, 0.f, Action->TargetsNumber);
	}

	return FCombatActionData();
}

void ABLCombatEnemy::HandleTurnsCooldown()
{
	TArray<int32> ActionsToDelete;

	for (auto& Action : ActionsCooldown)
	{
		--Action.Value;
		if (Action.Value <= 0)
		{
			ActionsToDelete.Add(Action.Key);
		}
	}

	for (const auto& Action : ActionsToDelete)
	{
		if (ActionsCooldown.Contains(Action))
		{
			ActionsCooldown.Remove(Action);
		}
	}
}
