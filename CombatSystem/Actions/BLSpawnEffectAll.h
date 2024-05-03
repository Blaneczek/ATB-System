// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BLAction.h"
#include "BLSpawnEffectAll.generated.h"

class APaperZDCharacter;

/**
 * 
 */
UCLASS(Blueprintable)
class BLADEOFLEGEND_API UBLSpawnEffectAll : public UBLAction
{
	GENERATED_BODY()

public:
	void ActivateAction(ABLCombatCharacter* Owner, const TArray<ABLCombatSlot*>& Targets) override;
	void ExecuteAction(ABLCombatCharacter* Owner, const TArray<ABLCombatSlot*>& Targets) override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BL|Combat")
	TSubclassOf<APaperZDCharacter> EffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BL|Combat")
	TObjectPtr<UPaperFlipbook> Effect;
};
