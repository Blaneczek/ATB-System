// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLCombatHero.h"

void ABLCombatHero::SneakAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("snekay attack status"));
	GiveStatus(ECombatStatusType::SNEAK, 1);
}
