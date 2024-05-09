// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BLAction.h"
#include "BLDefaultRangeAction.generated.h"

class ABLRangeProjectile;
class UPaperFlipbook;

/**
 * Default Range action. One projectile to one target. Only damage.
 */
UCLASS(Blueprintable)
class BLADEOFLEGEND_API UBLDefaultRangeAction : public UBLAction
{
	GENERATED_BODY()
	
public:
	void ActivateAction(UBLActionComponent* OwnerComponent) override;
	void ExecuteAction(ABLCombatSlot* Target) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BL|Combat")
	TSubclassOf<ABLRangeProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BL|Combat")
	TObjectPtr<UPaperFlipbook> ProjectileSprite;
};
