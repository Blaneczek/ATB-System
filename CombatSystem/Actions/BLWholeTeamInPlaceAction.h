// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BLAction.h"
#include "BLWholeTeamInPlaceAction.generated.h"

class APaperZDCharacter;

/**
 * 
 */
UCLASS(Blueprintable)
class BLADEOFLEGEND_API UBLWholeTeamInPlaceAction : public UBLAction
{
	GENERATED_BODY()

public:
	void ActivateAction(UBLActionComponent* OwnerComponent) override;
	void ExecuteAction(const TArray<ABLCombatSlot*>& Targets) override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BL|Combat")
	TSubclassOf<APaperZDCharacter> EffectClass;
};
