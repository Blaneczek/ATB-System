// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/BLCombatUtilities.h"
#include "BLStatusesComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLADEOFLEGEND_API UBLStatusesComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBLStatusesComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	/** Adds given status to Statuses map for a certain number of turns and performs status effect if it is immediate. */
	UFUNCTION(BlueprintCallable)
	void GiveStatus(ECombatStatusType Status, int32 Turns);

	/** Removes given status from Statuses map and removes status effect if it was previously applied. */
	UFUNCTION(BlueprintCallable)
	void RemoveStatus(ECombatStatusType Status);

	/**
	 * Checks whether any of the statuses are ready to be removed.
	 * Executes the status effect, if it is executed every turn.
	 */
	void HandleStatuses();

	/** Adds all statuses from Action and weapon that will be successively applied */
	void HandleHitStatuses(ABLCombatCharacter* Attacker, const TArray<FCombatStatus>& InStatuses, bool bMagicalAction);
	
public:
	UPROPERTY()
	TMap<ECombatStatusType, int32> Statuses;

};
