// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BLAction.h"
#include "BLMultipleInPlace.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class BLADEOFLEGEND_API UBLMultipleInPlace : public UBLAction
{
	GENERATED_BODY()

public:
	void ActivateAction(ABLCombatCharacter* Owner, const TArray<ABLCombatSlot*>& Targets) override;
	void ExecuteAction(ABLCombatCharacter* Owner, const TArray<ABLCombatSlot*>& Targets) override;
};
