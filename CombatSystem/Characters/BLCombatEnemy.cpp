// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLCombatEnemy.h"

void ABLCombatEnemy::SneakAttack()
{
	const float DMG = BaseData.MaxHP * 0.1;

	// There is a camera fade at the beginning of the combat so we need a small delay to see DMGDisplay widget.
	FTimerDelegate DelayDel;
	DelayDel.BindUObject(this, &ABLCombatEnemy::TakeSimpleDamage, DMG);
	FTimerHandle DelayTimer;
	GetWorld()->GetTimerManager().SetTimer(DelayTimer, DelayDel, 1.f, false);
}
