// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BLCombatCharacter.h"
#include "BLCombatHero.generated.h"

/**
 * 
 */
UCLASS()
class BLADEOFLEGEND_API ABLCombatHero : public ABLCombatCharacter
{
	GENERATED_BODY()

public:
	virtual void SneakAttackStatus() override;

	virtual void  StartActionTurnsCooldown(int32 TurnsCost) override;

protected:
	virtual void HandleTurnsCooldown() override;

private:
	UPROPERTY()
	TMap<TObjectPtr<UBLActionEntryData>, int32> ActionsTurnsCooldown;
};
