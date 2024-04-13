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

DECLARE_DELEGATE(FOnEndCooldown);
DECLARE_DELEGATE(FOnActionEnded);
DECLARE_DELEGATE(FOnHealthUpdate);
DECLARE_DELEGATE(FOnDeath);

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

public:
	virtual void Tick(float DeltaTime);

	void SetData(const FCombatCharData& InBaseData, const FCombatActions& InCombatActions, const FTransform& InSlotTransform);
	void SetData(const FCombatCharData& InBaseData, const TArray<TSoftClassPtr<UBLAction>>& InAttackActions, const TArray<TSoftClassPtr<UBLAction>>& InDefendActions);

	UFUNCTION(BlueprintCallable, meta = (AutoCreateRefTerm = "InStatuses"))
	virtual void HandleHitByAction(ABLCombatCharacter* Attacker, float Damage, ECombatElementType DamageElementType, bool bMagical, const TArray<ECombatStatus>& InStatuses);

	UFUNCTION()
	void StartCooldown();

	FTimerHandle GetCooldownTimer() { return CooldownTimer; };

	float GetCooldown() const { return BaseData.Cooldown; };
	float GetCurrentHP() const { return CurrentHP; };
	void SetCurrentHP(float NewHP) { CurrentHP = NewHP; };
	float GetMaxHP() const { return BaseData.MaxHP; };
	float GetCurrentME() const { return CurrentME; };
	void SetCurrentME(float NewME) { CurrentME = NewME; };
	float GetMaxME() const { return BaseData.MaxME; };

	UFUNCTION(BlueprintCallable)
	float GetAttackDMG() const { return BaseData.BaseAttackDMG; };
	UFUNCTION(BlueprintCallable)
	int32 GetBaseDefense() const { return BaseData.BaseDefense; };
	UFUNCTION(BlueprintCallable)
	void SetDefense(int32 NewDefense) { CurrentDefense = NewDefense; };
	
	/**
	* Activates/deactivates Defend idle animation
	* @param NewActive: true - active, false - inactive
	*/
	UFUNCTION(BlueprintCallable)
	void DefendIdleActivation(bool NewActiv) { bDefendIdle = NewActiv; };

	UFUNCTION(BlueprintCallable)
	bool IsDefendIdle() const { return bDefendIdle; };

	/** Only for text render (Debug) */
	UFUNCTION(BlueprintCallable)
	FString GetName() const { return BaseData.Name; };

	void CreateAction(const FVector& OwnerSlotLocation, const TArray<ABLCombatCharacter*>& Targets, const FCombatActionData& ActionData);

	/** Action is executing in place, no targets */
	void DefaultAction();
	/** Character runs up to the target and executes action */
	void DefaultMeleeAction();
	/** Character creates a projectile that flies to the target and executes action */
	void DefaultRangeAction(TSubclassOf<ABLRangeProjectile> ProjectileClass, UPaperFlipbook* ProjectileSprite);
	/** Character runs up to the targets one by one and executes action for every target */
	void MultipleDefaultMeleeAction();
	/** Character creates multiple projectiles that flie to the target and execute action */
	void MultipleDefaultRangeAction(TSubclassOf<ABLRangeProjectile> ProjectileClass, UPaperFlipbook* ProjectileSprite);
	/** Character runs up to the column and executes action for every target in column */
	void ColumnMeleeAction();


	/** Special actions turns cooldown */
	void StartActionCooldown(int32 TurnsCost);

	/** Shows taken dmg/healed value*/
	UFUNCTION(BlueprintImplementableEvent)
	void DisplayTextDMG(float DMG, bool bHeal, ECombatElementType DMGElement, bool bDodge = false);

	/** Moves selected Hero forward */
	//void StepForward();
	/** Moves selected Hero back in line */
	//void BackInLine();

private:
	float CalculateElementsMultipliers(ECombatElementType DamageElementType, ECombatElementType CharacterElementType, bool& OutIsHeal);

	UFUNCTION()
	void EndCooldown();
	UFUNCTION()
	void EndAction(bool bResult);

	void ReachedActionDestination(FAIRequestID RequestID, const FPathFollowingResult& Result);
	void ReachedActionDestination(FAIRequestID RequestID, const FPathFollowingResult& Result, int32 TargetIndex);
	void ReachedActionDestination();
	void ReachedActionDestination(int32 Index, bool bLastProjectile);
	void ReachedSlotLocation(FAIRequestID RequestID, const FPathFollowingResult& Result);

	void SpawnProjectile(TSubclassOf<ABLRangeProjectile> ProjectileClass, UPaperFlipbook* ProjectileSprite);

	void GiveStatus(ECombatStatus Status);
	void RemoveStatus(ECombatStatus Status);
	void HandleStatuses();

	void TakeSimpleDamage(float Damage);

public:
	FOnEndCooldown OnEndCooldown;
	FOnActionEnded OnActionEnded;
	FOnHealthUpdate OnHealthUpdate;
	FOnDeath OnDeath;

protected:
	UPROPERTY()
	bool bDefendIdle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BL|Combat")
	FCombatCharData BaseData;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BL|Combat")
	float CurrentHP;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BL|Combat")
	float CurrentME;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BL|Combat")
	int32 CurrentDefense;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BL|Combat")
	TObjectPtr<UWidgetComponent> DMGDisplay;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BL|Combat")
	TObjectPtr<UWidgetComponent> BleedingDisplay;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BL|Combat")
	TObjectPtr<UWidgetComponent> PoisoningDisplay;

	UPROPERTY()
	FTimerHandle CooldownTimer;

	UPROPERTY()
	TMap<ECombatStatus, int32> Statuses;

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
	TArray<TSoftClassPtr<UBLAction>> AttackActions;
	UPROPERTY()
	TArray<TSoftClassPtr<UBLAction>> DefendActions;
	UPROPERTY()
	TMap<ECrystalColor, FCrystalSkills> CrystalActions;
	UPROPERTY()
	TArray<TSoftClassPtr<UBLAction>> SpecialActions;
	UPROPERTY()
	TArray<FCombatItems> ItemActions;

	UPROPERTY()
	TMap<TObjectPtr<UBLActionEntryData>, int32> ActionsTurnsCooldown;
	UPROPERTY()
	TObjectPtr<UBLActionEntryData> ClickedActionEntry;

	/** Counter for multiple projectiles to now when ends action*/
	UPROPERTY()
	int32 ProjectileTargetsNum;
	UPROPERTY()
	int32 ProjectileTargetIndex;

	FTimerHandle ProjectileSpawnTimer;
};
