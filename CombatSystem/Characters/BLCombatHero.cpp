// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLCombatHero.h"
#include "UI/Entries/BLActionEntryData.h"

void ABLCombatHero::SneakAttackStatus()
{
	GiveStatus(ECombatStatusType::SNEAK, 1);
}

void ABLCombatHero::StartActionTurnsCooldown(int32 TurnsCost)
{
	if (ClickedActionEntry)
	{
		ClickedActionEntry->bCanBeUsed = false;
		ActionsTurnsCooldown.Add(ClickedActionEntry, TurnsCost);
	}
}

void ABLCombatHero::HandleTurnsCooldown()
{
	TArray<UBLActionEntryData*> ActionsToDelete;

	for (auto& Action : ActionsTurnsCooldown)
	{
		if (Action.Key->bCanBeUsed) continue;

		--Action.Value;
		Action.Key->ChangeName(Action.Value);
		if (Action.Value <= 0)
		{
			Action.Key->ChangeNameToDefault();
			Action.Key->bCanBeUsed = true;
			ActionsToDelete.Add(Action.Key);
		}
	}

	for (const auto* Action : ActionsToDelete)
	{
		if (ActionsTurnsCooldown.Contains(Action))
		{
			ActionsTurnsCooldown.Remove(Action);
		}
	}
}

