// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BladeOfLegend/AREK/Character/BLBaseChar.h"
#include "BLCombatUtilities.h"
#include "AIController.h"
#include "Actions/BLActionsInterface.h"
#include "BLCombatCharacter.generated.h"

class USphereComponent;
class UWidgetComponent;
class UPaperZDAnimInstance;
class UPaperFlipbook;
class ABLRangeProjectile;
class UBLAction;
class UBLActionEntryData;
class ABLCombatSlot;

DECLARE_DELEGATE(FOnEndCooldown);
DECLARE_DELEGATE(FOnActionEnded);
DECLARE_DELEGATE(FOnHealthUpdate);
DECLARE_DELEGATE(FOnDeath);
DECLARE_DELEGATE_OneParam(FOnEscape, bool /*bSuccessful*/);

/**
 * 
 */
UCLASS()
class BLADEOFLEGEND_API ABLCombatCharacter : public ABLBaseChar
{
	GENERATED_BODY()

public:
	ABLCombatCharacter();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime);

	void SetData(const FCombatCharData& InBaseData, const FCombatActions& InCombatActions, const FTransform& InSlotTransform);
	void SetData(const FCombatCharData& InBaseData, const TArray<TSoftClassPtr<UBLAction>>& InActions);

	UFUNCTION(BlueprintCallable, meta = (AutoCreateRefTerm = "InStatuses"))
	virtual void HandleHitByAction(ABLCombatCharacter* Attacker, float Damage, ECombatElementType DamageElementType, bool bMagical, const TArray<FCombatStatus>& InStatuses);

	UFUNCTION()
	void StartCooldown();

	const FTimerHandle& GetCooldownTimer() const { return CooldownTimer; };

	UFUNCTION(BlueprintCallable)
	float GetCooldown() const { return CurrentCooldown; };

	UFUNCTION(BlueprintCallable)
	float GetCurrentHP() const { return CurrentHP; };

	UFUNCTION(BlueprintCallable)
	void SetCurrentHP(float NewHP) { CurrentHP = NewHP; };

	UFUNCTION(BlueprintCallable)
	float GetMaxHP() const { return BaseData.MaxHP; };

	UFUNCTION(BlueprintCallable)
	float GetCurrentME() const { return CurrentME; };

	UFUNCTION(BlueprintCallable)
	void SetCurrentME(float NewME) { CurrentME = NewME; };

	UFUNCTION(BlueprintCallable)
	float GetMaxME() const { return BaseData.MaxME; };

	UFUNCTION(BlueprintCallable)
	ECombatElementType GetWeaponElement() const { return BaseData.WeaponElement; };

	UFUNCTION(BlueprintCallable)
	const FCombatStatus& GetWeaponStatus() const { return BaseData.WeaponStatus; };

	UFUNCTION(BlueprintCallable)
	float GetAttackDMG() const { return CurrentAttackDMG; };

	UFUNCTION(BlueprintCallable)
	float GetCurrentDefense() const { return CurrentDefense; };

	UFUNCTION(BlueprintCallable)
	void SetCurrentDefense(float NewDefense) { CurrentDefense = NewDefense; };

	UFUNCTION(BlueprintCallable)
	const FCombatCharData& GetBaseData() const { return BaseData; };

	UFUNCTION(BlueprintCallable)
	bool IsDead() const { return bDead; };

	UFUNCTION(BlueprintCallable)
	FString GetName() const { return BaseData.Name; };

	UFUNCTION(BlueprintCallable)
	TSubclassOf<UPaperZDAnimInstance> GetCombatAnimClass() const { return BaseData.AnimInstanceClass; };

	void CreateAction(const FVector& OwnerSlotLocation, const TArray<ABLCombatCharacter*>& Targets, const FCombatActionData& ActionData, AActor* CombatManager);

	void CreateAction(const FVector& OwnerSlotLocation, const TArray<ABLCombatSlot*>& Targets, const FCombatActionData& ActionData, AActor* CombatManager);

	/********************* 
	*  ACTION FLOW TYPES 
	**********************/

	/** Action is executing in place, no targets */
	void DefaultAction();

	/** Character runs up to the target and executes action */
	void DefaultMeleeAction();

	/** Character creates a projectile that flies to the target and executes action */
	void DefaultRangeAction(TSubclassOf<ABLRangeProjectile> ProjectileClass, UPaperFlipbook* ProjectileSprite);

	/** Character runs up to the targets one by one and executes action for every target */
	void MultipleDefaultMeleeAction();

	/** Character creates multiple projectiles that fly to the targets and execute action */
	void MultipleDefaultRangeAction(TSubclassOf<ABLRangeProjectile> ProjectileClass, UPaperFlipbook* ProjectileSprite);

	/** Character runs up to the column and executes action for every target in column */
	void ColumnMeleeAction();

	/** Character runs up to the column and executes action for every target in column */
	void BounceRangeAction(TSubclassOf<ABLRangeProjectile> ProjectileClass, UPaperFlipbook* ProjectileSprite);

	void SummonAction(const TArray<ABLCombatSlot*>& Targets);

	void SpawnEffectsAllAction(const TArray<ABLCombatSlot*>& Targets, TSubclassOf<APaperZDCharacter> EffectClass, UPaperFlipbook* Effect);
	/***********************************************************************************************/

	/** Special actions turns cooldown */
	void StartActionCooldown(int32 TurnsCost);

	/** Shows taken dmg/healed value*/
	UFUNCTION(BlueprintImplementableEvent)
	void DisplayTextDMG(float DMG, bool bHeal, ECombatElementType DMGElement, bool bDodge = false);

	UFUNCTION(BlueprintCallable)
	void GiveStatus(ECombatStatusType Status, int32 Turns);

	/**
	* Adds/Removes given status icon
	* @param bNewVisibility: true - add, false - remove
	*/
	UFUNCTION(BlueprintImplementableEvent, meta = (AutoCreateRefTerm = "Status"))
	void SetStatusDisplayVisibility(ECombatStatusType Status, bool bNewVisibility);

	/** Removes given status from Statuses */
	UFUNCTION()
	void RemoveStatus(ECombatStatusType Status);

	UFUNCTION()
	void RemoveStatuses(TArray<ECombatStatusType> StatusesToDelete);

	virtual void SneakAttack() {};

	/** Handles the situation where Action damage will be converted into healing or used action with heal*/
	UFUNCTION(BlueprintCallable)
	void HandleHealHit(float Damage, float HealMultiplier, ECombatElementType HealElementType);

	virtual FCombatActionData GetEnemyAction() { return FCombatActionData(); }


	UFUNCTION(BlueprintCallable)
	bool IsDefendIdle() const{ return bDefendIdle; }

protected:
	void TakeSimpleDamage(float Damage);

	virtual void HandleTurnsCooldown() {};

private:
	float CalculateElementsMultipliers(ECombatElementType DamageElementType, ECombatElementType CharacterElementType, bool& OutIsHeal);

	UFUNCTION()
	void EndCooldown();
	UFUNCTION()
	void EndAction(bool bResult);

	/** For DefaultMelee and ColumnMelee action flow*/
	void ReachedActionDestination(FAIRequestID RequestID, const FPathFollowingResult& Result);

	/** For MultipleMelee action flow */
	void ReachedActionDestination(FAIRequestID RequestID, const FPathFollowingResult& Result, int32 TargetIndex);

	/** For DeafultRange action flow */
	void ReachedActionDestination();

	/** For MultipleRange action flow */
	void ReachedActionDestination(int32 Index, bool bLastProjectile);

	/** For BounceRange action flow */
	void ReachedActionDestination(ABLRangeProjectile* Projectile, int32 Index);

	/** Back to the owner slot after action */
	void ReachedSlotLocation(FAIRequestID RequestID, const FPathFollowingResult& Result);

	void SpawnProjectile(TSubclassOf<ABLRangeProjectile> ProjectileClass, UPaperFlipbook* ProjectileSprite);

	void HandleStatuses();

	/** Handles the situation where Action deals damage to a character */
	void HandleDamageHit(ABLCombatCharacter* Attacker, float Damage, float DMGMultiplier, ECombatElementType DamageElementType, bool bMagicalAction);

	/** Adds all statuses from Action and weapon that will be successively applied */
	void HandleHitStatuses(ABLCombatCharacter* Attacker, const TArray<FCombatStatus>& InStatuses, bool bMagicalAction);

public:
	FOnEndCooldown OnEndCooldown;
	FOnActionEnded OnActionEnded;
	FOnHealthUpdate OnHealthUpdate;
	FOnDeath OnDeath;
	FOnEscape OnEscape;

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

protected:
	/** If idle Death animation should be played */
	UPROPERTY()
	bool bDead;

	UPROPERTY()
	bool bMagicImmunity = false;

	UPROPERTY()
	FCombatCharData BaseData;

	UPROPERTY()
	float CurrentHP;

	UPROPERTY()
	float CurrentME;

	UPROPERTY()
	float CurrentDefense;

	UPROPERTY()
	float CurrentCooldown;

	UPROPERTY()
	float CurrentDodge;

	UPROPERTY()
	float CurrentAttackDMG;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BL|Combat")
	TObjectPtr<UWidgetComponent> DMGDisplay;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BL|Combat")
	TObjectPtr<UWidgetComponent> StatusDisplay;

	UPROPERTY()
	FTimerHandle CooldownTimer;

	UPROPERTY()
	TMap<ECombatStatusType, int32> Statuses;

	UPROPERTY()
	TMap<TObjectPtr<UBLActionEntryData>, int32> ActionsTurnsCooldown;

	/// 
	UPROPERTY()
	bool bDefendIdle = false;

private:
	UPROPERTY()
	TObjectPtr<AAIController> AIC;

	UPROPERTY()
	TArray<TObjectPtr<ABLCombatCharacter>> TargetCharacters;

	/** Pointer to created Action to prevent GC while it is still running. Nulled after end of action */
	UPROPERTY()
	TObjectPtr<UBLAction> CurrentAction;

	UPROPERTY()
	FVector SlotLocation;

	UPROPERTY()
	FTransform SlotTransform;

	UPROPERTY()
	TObjectPtr<UBLActionEntryData> ClickedActionEntry;

	/** Counter for multiple projectiles to know when ends action*/
	UPROPERTY()
	int32 ProjectileTargetsNum;

	UPROPERTY()
	int32 ProjectileTargetIndex;

	FTimerHandle ProjectileSpawnTimer;
};
