// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLAction.h"

void UBLAction::OnCreateAction(ABLCombatCharacter* Owner)
{
	if (Owner)
	{
		ActivateAction(Owner);
	}	
}

void UBLAction::EndAction()
{
	//
}




