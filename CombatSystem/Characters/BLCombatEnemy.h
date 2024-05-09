// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BLCombatCharacter.h"
#include "BLCombatEnemy.generated.h"

/**
 * 
 */
UCLASS()
class BLADEOFLEGEND_API ABLCombatEnemy : public ABLCombatCharacter
{
	GENERATED_BODY()

public:
	virtual void SneakAttackStatus() override;

	virtual FCombatActionData GetEnemyAction() override;

protected:
	virtual void HandleTurnsCooldown() override;

private:
	UPROPERTY()
	TMap<int32, int32> ActionsCooldown;
};
