// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLCombatHero.h"

void ABLCombatHero::DefendAction()
{
	Super::DefendAction();

	OnHealthUpdate.ExecuteIfBound();
}
