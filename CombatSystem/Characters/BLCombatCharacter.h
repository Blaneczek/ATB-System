// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BladeOfLegend/AREK/Character/BLBaseChar.h"
#include "BLCombatUtilities.h"
#include "AIController.h"
#include "BLCombatCharacter.generated.h"

class USphereComponent;
class UPaperZDAnimInstance;
class UPaperFlipbook;
class ABLRangeProjectile;

DECLARE_DELEGATE(FOnStartCooldown);
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

	void SetData(const FCombatCharData& InBaseData, const FAttackActionData& InAttackData, const FDefendActionData& InDefendData);

	virtual void HandleHitByAction(float Damage, ECombatElementType DamageElementType);

	virtual void AttackAction(const FVector& OwnerSlotLocation, ABLCombatCharacter* Target);
	virtual void DefendAction();
	virtual void CrystalSkillAction() {};
	virtual void SpecialSkillAction();
	virtual void ItemAction() {};

	UFUNCTION()
	void StartCooldown();

	FTimerHandle GetCooldownTimer() { return CooldownTimer; };

	float GetCooldown() const { return BaseData.Cooldown; };
	float GetCurrentHP() const { return CurrentHP; };
	float GetMaxHP() const { return BaseData.MaxHP; };
	float GetCurrentME() const { return CurrentME; };
	float GetMaxME() const { return BaseData.MaxME; };

private:
	float CalculateElementsMultipliers(ECombatElementType DamageElementType, ECombatElementType CharacterElementType, bool& OutIsHeal);

	
	UFUNCTION()
	void EndCooldown();
	UFUNCTION()
	void EndAction(bool bResult);

	void ReachedAttackDestination(FAIRequestID RequestID, const FPathFollowingResult& Result);
	void ReachedSlotLocation(FAIRequestID RequestID, const FPathFollowingResult& Result);

public:
	FOnStartCooldown OnStartCooldown;
	FOnEndCooldown OnEndCooldown;
	FOnActionEnded OnActionEnded;
	FOnHealthUpdate OnHealthUpdate;
	FOnDeath OnDeath;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BL|Combat")
	bool bDefendIdle;

	//only for text render
	UFUNCTION(BlueprintCallable)
	FString GetName() const { return BaseData.Name; };

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BL|Combat")
	FCombatCharData BaseData;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BL|Combat")
	float CurrentHP;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BL|Combat")
	float CurrentME;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BL|Combat")
	float CurrentDefense;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BL|Combat")
	FAttackActionData AttackActionData;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BL|Combat")
	FDefendActionData DefendActionData;

	UPROPERTY()
	FTimerHandle CooldownTimer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BL|Combat")
	TSubclassOf<ABLRangeProjectile> ProjectileClass;

private:
	UPROPERTY()
	TObjectPtr<AAIController> AIC;

	UPROPERTY()
	TObjectPtr<ABLCombatCharacter> TargetCharacter;
	UPROPERTY()
	FVector SlotLocation;
};
