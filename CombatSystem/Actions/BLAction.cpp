// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLAction.h"
#include "Characters/BLCombatCharacter.h"

void UBLAction::OnCreateAction(ABLCombatCharacter* Owner)
{
	if (Owner)
	{
		ActivateAction(Owner);

		if (TurnsCost > 0)
		{
			Owner->StartActionCooldown(TurnsCost);
		}
	}	
}






