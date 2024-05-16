// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLAction.h"
#include "CombatComponents/BLActionComponent.h"
#include "Characters/BLCombatCharacter.h"

void UBLAction::OnCreateAction(UBLActionComponent* OwnerComponent, ABLCombatCharacter* Owner, AActor* InCombatManager)
{
	if (OwnerComponent && Owner)
	{
		CombatManager = InCombatManager;
		OwnerChar = Owner;

		ActivateAction(OwnerComponent);

		if (TurnsCost > 0)
		{
			OwnerChar->StartActionTurnsCooldown(TurnsCost);
		}
	}
}







