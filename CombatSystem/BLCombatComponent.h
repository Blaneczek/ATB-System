// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BLCombatUtilities.h"
#include "BLCombatComponent.generated.h"

/** Component given to enemy in Overworld, storing all the necessary data about the combat. */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class BLADEOFLEGEND_API UBLCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBLCombatComponent();

protected:
	virtual void BeginPlay() override;

public:
	/** Sets all data to game instance and opens the appropriate map. */
	UFUNCTION(BlueprintCallable)
	void StartCombat();

	/** Sets the flag if it was a sneak attack (the player attacked first in Overworld). */
	UFUNCTION(BlueprintCallable)
	void SneakAttack();

	/** Adds to Post combata data exp and gold from enemy. */
	UFUNCTION(BlueprintCallable)
	void AddExpAndGold(int32 Exp, int32 Gold);

public:
	/** All information about the combat, including data about enemy team. The enemy team can be set manually or generated (function in the blueprint). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCombatData CombatData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPostCombatData PostCombatData;
};
