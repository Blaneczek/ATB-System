// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BLCombatUtilities.h"
#include "AIController.h"
#include "BLActionComponent.generated.h"

class ABLRangeProjectile;
class UPaperFlipbook;
class ABLCombatSlot;
class APaperZDCharacter;

DECLARE_DELEGATE(FOnActionFinished);
DECLARE_DELEGATE_OneParam(FOnEscapeAction, bool /*bSuccessful*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLADEOFLEGEND_API UBLActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBLActionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	void CreateAction(const FVector& OwnerSlotLocation, const TArray<ABLCombatSlot*>& Targets, const FCombatActionData& ActionData, AActor* CombatManagerActor);

	void SetActions(const FCombatActions& InCombatActions);

	void SetActions(const TArray<TSoftClassPtr<UBLAction>>& InActions);

	/*********************
	*  ACTION FLOW TYPES
	**********************/

	/** Action is executing in place, target is the owner. */
	void DefaultAction();

	/** Character runs up to the target and executes action. */
	void DefaultMeleeAction();

	/** Character creates a projectile that flies to the target and executes action. */
	void DefaultRangeAction(TSubclassOf<ABLRangeProjectile> ProjectileClass, UPaperFlipbook* ProjectileSprite);

	/** Character runs up to the targets one by one and executes action for every target .*/
	void MultipleDefaultMeleeAction();

	/** Character creates multiple projectiles that fly to the targets and execute action. */
	void MultipleDefaultRangeAction(TSubclassOf<ABLRangeProjectile> ProjectileClass, UPaperFlipbook* ProjectileSprite);

	/** Character runs up to the column and executes an action for every target in column. */
	void ColumnMeleeAction();

	/** Character creates a single projectile that travels through multiple targets. */
	void BounceRangeAction(TSubclassOf<ABLRangeProjectile> ProjectileClass, UPaperFlipbook* ProjectileSprite);

	/** Character in place executes an action for each target. */
	void MultipleInPlaceAction(TSubclassOf<APaperZDCharacter> EffectClass);

	/***********************************************************************************************/

private:
	/**
	 * For DefaultMelee and ColumnMelee action flow.
	 * Character executes action and returns to the slot after approaching target.
	 */
	void ReachedActionDestination(FAIRequestID RequestID, const FPathFollowingResult& Result);

	/**
	 * For MultipleMelee action flow
	 * Character executes action, if there is another target, the character approaches it, if not, he returns to the slot.
	 * @param TargetIndex: Index of the approached target.
	 */
	void ReachedActionDestination(FAIRequestID RequestID, const FPathFollowingResult& Result, int32 TargetIndex);

	/**
	 * For DeafultRange action flow
	 * Executes action after the projectile reaches the target.
	 */
	void ReachedActionDestination();

	/** For MultipleRange action flow 
	 *  Ends action when the last projectile reaches its target.
	 */
	void ReachedActionDestination(int32 Index, bool bLastProjectile);

	/** For BounceRange action flow
	 *  If there is another target, sents the projectile. If not, ends action.
	 */
	void ReachedActionDestination(ABLRangeProjectile* Projectile, int32 Index);

	/** Back to the owner slot after action */
	void ReachedSlotLocation(FAIRequestID RequestID, const FPathFollowingResult& Result);

	/** Used in multiple range action. Spawns and sends a projectile to the target. */
	void SpawnProjectile(TSubclassOf<ABLRangeProjectile> ProjectileClass, UPaperFlipbook* ProjectileSprite);

	UFUNCTION()
	void EndAction(bool bResult);

public:
	FOnActionFinished OnActionFinished;
	FOnEscapeAction OnEscapeAction;

	UPROPERTY()
	TArray<TSoftClassPtr<UBLAction>> AttackActions;

	UPROPERTY()
	TSoftClassPtr<UBLAction> DefendAction;
	UPROPERTY()
	TMap<ECrystalColor, FCrystalSkills> CrystalActions;

	UPROPERTY()
	TArray<TSoftClassPtr<UBLAction>> SpecialActions;

	UPROPERTY()
	TArray<TSoftClassPtr<UBLCombatItem>> ItemActions;

	UPROPERTY()
	TSoftClassPtr<UBLAction> RunAwayAction;

private:
	UPROPERTY()
	TObjectPtr<AAIController> AIC;

	UPROPERTY()
	TArray<TObjectPtr<ABLCombatSlot>> TargetSlots;

	/** Pointer to created Action to prevent GC while it is still running. Nulled after end of action. */
	UPROPERTY()
	TObjectPtr<UBLAction> CurrentAction;

	UPROPERTY()
	FVector SlotLocation;

	/******* Variables needed for multiple range action. *******/

	/** Counter for multiple projectiles to know when ends action. */
	int32 ProjectileTargetsNum;

	int32 ProjectileTargetIndex;
	FTimerHandle ProjectileSpawnTimer;
};
