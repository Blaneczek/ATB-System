// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BladeOfLegend/AREK/Character/BLBaseChar.h"
#include "BLCombatUtilities.h"
#include "AIController.h"
#include "Actions/BLActionsInterface.h"
#include "BLCombatCharacter.generated.h"

class USphereComponent;
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

	void SetData(const FCombatCharData& InBaseData, const TArray<TSoftClassPtr<UBLAction>>& InAttackActions, const TArray<TSoftClassPtr<UBLAction>>& InDefendActions);

	virtual void HandleHitByAction(float Damage, ECombatElementType DamageElementType);

	UFUNCTION()
	void StartCooldown();

	FTimerHandle GetCooldownTimer() { return CooldownTimer; };

	float GetCooldown() const { return BaseData.Cooldown; };
	float GetCurrentHP() const { return CurrentHP; };
	float GetMaxHP() const { return BaseData.MaxHP; };
	float GetCurrentME() const { return CurrentME; };
	float GetMaxME() const { return BaseData.MaxME; };

	void CreateAction(const FVector& OwnerSlotLocation, ECombatActionType ActionType, int32 ActionIndex, ABLCombatCharacter* Target);

	/** Action is executing in place, no targets */
	void DefaultAction();
	/** Character runs up to the target and executes action */
	void DefaultMeleeAction();
	/** Character creates a projectile that flies to the target and executes action */
	void DefaultRangeAction(TSubclassOf<ABLRangeProjectile> ProjectileClass, UPaperFlipbook* ProjectileSprite);

private:
	float CalculateElementsMultipliers(ECombatElementType DamageElementType, ECombatElementType CharacterElementType, bool& OutIsHeal);

	
	UFUNCTION()
	void EndCooldown();
	UFUNCTION()
	void EndAction(bool bResult);

	void ReachedActionDestination(FAIRequestID RequestID, const FPathFollowingResult& Result);
	void ReachedActionDestination();
	void ReachedSlotLocation(FAIRequestID RequestID, const FPathFollowingResult& Result);

public:
	FOnEndCooldown OnEndCooldown;
	FOnActionEnded OnActionEnded;
	FOnHealthUpdate OnHealthUpdate;
	FOnDeath OnDeath;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BL|Combat")
	bool bDefendIdle;

	//only for text render
	UFUNCTION(BlueprintCallable)
	FString GetName() const { return BaseData.Name; };

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BL|Combat")
	FCombatCharData BaseData;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BL|Combat")
	float CurrentHP;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BL|Combat")
	float CurrentME;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BL|Combat")
	float CurrentDefense;
	

protected:
	UPROPERTY()
	FTimerHandle CooldownTimer;

private:
	UPROPERTY()
	TObjectPtr<AAIController> AIC;

	UPROPERTY()
	TObjectPtr<ABLCombatCharacter> TargetCharacter;

	/** Pointer to created Action to prevent GC while it is still running. Nulled after end of action */
	UPROPERTY()
	TObjectPtr<UBLAction> CurrentAction;

	UPROPERTY()
	FVector SlotLocation;

	UPROPERTY()
	TArray<TSoftClassPtr<UBLAction>> AttackActions;
	UPROPERTY()
	TArray<TSoftClassPtr<UBLAction>> DefendActions;
};
