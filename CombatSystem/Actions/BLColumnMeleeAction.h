// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BLAction.h"
#include "BLColumnMeleeAction.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class BLADEOFLEGEND_API UBLColumnMeleeAction : public UBLAction
{
	GENERATED_BODY()

public:
	void ActivateAction(ABLCombatCharacter* Owner) override;
	void ExecuteAction(ABLCombatCharacter* Owner, ABLCombatCharacter* Target) override;

private:
	/** To track when the last target in column will get hit by action and then end action*/
	bool bEndAction = true;
};
