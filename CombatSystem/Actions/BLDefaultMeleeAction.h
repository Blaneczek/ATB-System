// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BLAction.h"
#include "BLDefaultMeleeAction.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class BLADEOFLEGEND_API UBLDefaultMeleeAction : public UBLAction
{
	GENERATED_BODY()

public:
	void ActivateAction(UBLActionComponent* OwnerComponent) override;

	void ExecuteAction(const TArray<ABLCombatSlot*>& Targets) override;

};
