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

	void SetData(const FCombatCharData& InBaseData, const TArray<TSoftClassPtr<UBLAction>>& InAttackActions, const TArray<TSoftClassPtr<UBLAction>>& InDefendActions, const TMap<ECrystalColor, FCrystalSkills>& InCrystalActions, const TArray<TSoftClassPtr<UBLAction>>& InSpecialActions);
	void SetData(const FCombatCharData& InBaseData, const TArray<TSoftClassPtr<UBLAction>>& InAttackActions, const TArray<TSoftClassPtr<UBLAction>>& InDefendActions);

	UFUNCTION(BlueprintCallable)
	virtual void HandleHitByAction(float Damage, ECombatElementType DamageElementType);

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
	float GetAttackDMG() const { return BaseData.AttackDMG; };
	UFUNCTION(BlueprintCallable)
	float GetBaseDefense() const { return BaseData.BaseDefense; };
	UFUNCTION(BlueprintCallable)
	void SetDefense(float NewDefense) { CurrentDefense = NewDefense; };
	
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

	void CreateAction(const FVector& OwnerSlotLocation, ECombatActionType ActionType, int32 ActionIndex, const TArray<ABLCombatCharacter*>& Targets, ECrystalColor CrystalColor = ECrystalColor::NONE);

	/** Action is executing in place, no targets */
	void DefaultAction();
	/** Character runs up to the target and executes action */
	void DefaultMeleeAction();
	/** Character creates a projectile that flies to the target and executes action */
	void DefaultRangeAction(TSubclassOf<ABLRangeProjectile> ProjectileClass, UPaperFlipbook* ProjectileSprite);
	/** Character runs up to the targets one by one and executes action for every target */
	void MultipleDefaultMeleeAction();

private:
	float CalculateElementsMultipliers(ECombatElementType DamageElementType, ECombatElementType CharacterElementType, bool& OutIsHeal);

	
	UFUNCTION()
	void EndCooldown();
	UFUNCTION()
	void EndAction(bool bResult);

	/** Default teached*/
	void ReachedActionDestination(FAIRequestID RequestID, const FPathFollowingResult& Result);
	void ReachedActionDestination(FAIRequestID RequestID, const FPathFollowingResult& Result, int32 TargetIndex);
	void ReachedActionDestination();
	void ReachedSlotLocation(FAIRequestID RequestID, const FPathFollowingResult& Result);

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
	float CurrentDefense;
	
	UPROPERTY()
	FTimerHandle CooldownTimer;

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
	TArray<TSoftClassPtr<UBLAction>> AttackActions;
	UPROPERTY()
	TArray<TSoftClassPtr<UBLAction>> DefendActions;
	UPROPERTY()
	TMap<ECrystalColor, FCrystalSkills> CrystalActions;
	UPROPERTY()
	TArray<TSoftClassPtr<UBLAction>> SpecialActions;

};
