// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BLAction.h"
#include "BLMultipleDefaultMeleeAction.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class BLADEOFLEGEND_API UBLMultipleDefaultMeleeAction : public UBLAction
{
	GENERATED_BODY()

public:
	void ActivateAction(ABLCombatCharacter* Owner) override;
	void ExecuteAction(ABLCombatCharacter* Owner, ABLCombatCharacter* Target) override;
};
