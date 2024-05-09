// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BladeOfLegend/AREK/Character/BLBaseChar.h"
#include "BLCombatUtilities.h"
#include "AIController.h"
#include "BLCombatCharacter.generated.h"

class USphereComponent;
class UWidgetComponent;
class UPaperZDAnimInstance;
class UPaperFlipbook;
class ABLRangeProjectile;
class UBLAction;
class UBLActionEntryData;
class ABLCombatSlot;
class UBLActionComponent;

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

	void SetData(const FCombatCharData& InBaseData);

	void SetHeroData(const FCombatCharData& InBaseData, const FCombatActions& InCombatActions);
	
	void SetEnemyData(const FCombatCharData& InBaseData, const TArray<TSoftClassPtr<UBLAction>>& InActions);

	/** Manages the situation when a character is hit by an action. */
	UFUNCTION(BlueprintCallable, meta = (AutoCreateRefTerm = "InStatuses"))
	virtual void HandleHitByAction(ABLCombatCharacter* Attacker, float Damage, ECombatElementType DamageElementType, bool bMagical, const TArray<FCombatStatus>& InStatuses);

	/** Handles the situation where Action damage will be converted into healing or used action with heal*/
	UFUNCTION(BlueprintCallable)
	void HandleHealHit(float Damage, float HealMultiplier, ECombatElementType HealElementType);	

	void CreateAction(const FVector& OwnerSlotLocation, const TArray<ABLCombatSlot*>& Targets, const FCombatActionData& ActionData, AActor* CombatManager);

	UFUNCTION()
	void StartCooldown();
	/** Hero's Actions turns cooldown */
	virtual void StartActionTurnsCooldown(int32 TurnsCost) {};

	/*********************
	*	   STATUSES
	**********************/

	/** Adds given status to Statuses map for a certain number of turns and performs status effect if it is immediate. */
	UFUNCTION(BlueprintCallable)
	void GiveStatus(ECombatStatusType Status, int32 Turns);

	/** Removes given status from Statuses map and removes status effect if it was previously applied. */
	UFUNCTION(BlueprintCallable)
	void RemoveStatus(ECombatStatusType Status);

	/**
	 * Adds/Removes given status icon. Implementable in blueprint.
	 * @param bNewVisibility: true - add, false - remove
	 */
	UFUNCTION(BlueprintImplementableEvent, meta = (AutoCreateRefTerm = "Status"))
	void SetStatusDisplayVisibility(ECombatStatusType Status, bool bNewVisibility);
	
	virtual void SneakAttackStatus() {};

	/***********************************************************************************************/


	/** Shows taken dmg/healed value in widget. Implementable in character blueprint.*/
	UFUNCTION(BlueprintImplementableEvent)
	void DisplayTextDMG(float DMG, bool bHeal, ECombatElementType DMGElement, bool bDodge = false);

	virtual FCombatActionData GetEnemyAction() { return FCombatActionData(); }

	UFUNCTION(BlueprintCallable)
	bool IsDefendIdle() const{ return bDefendIdle; }

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

protected:
	virtual void HandleTurnsCooldown() {};

	/** Applies simple damage that doesn't need to be calculated. */
	void TakeSimpleDamage(float Damage);

private:
	/** 
	 * Calculates the damage multiplier depending on the combination of elements. 
	 * @param OutIsHeal - if elements are of the same type, it will heal the character (except NONE-NONE). 
	 */
	float CalculateElementsMultipliers(ECombatElementType DamageElementType, ECombatElementType CharacterElementType, bool& OutIsHeal);

	UFUNCTION()
	void EndCooldown();

	/** 
	 * Checks whether any of the statuses are ready to be removed.
	 * Executes the status effect, if it is executed every turn. 
	 */
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
	bool bDefendIdle = false;

	UPROPERTY()
	TObjectPtr<UBLActionEntryData> ClickedActionEntry;

	/* For enemies to choose actions */
	UPROPERTY()
	TArray<TSoftClassPtr<UBLAction>> AttackActions;

	UPROPERTY()
	TObjectPtr<UBLActionComponent> ActionComponent;
};
