// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BLCombatUtilities.h"
#include "BLCombatSlot.generated.h"

class ABLCombatCharacter;
class UBoxComponent;

DECLARE_DELEGATE_OneParam(FOnSelectedSlot, ABLCombatSlot* /*Slot*/);
DECLARE_DELEGATE_TwoParams(FOnCharActionEnded, ABLCombatSlot* /*Slot*/, bool /*bIsEnemy*/);
DECLARE_DELEGATE_OneParam(FOnCharHealthUpdate, ABLCombatSlot* /*Slot*/);
DECLARE_DELEGATE_TwoParams(FOnCharDeath, ABLCombatSlot* /*Slot*/, bool /*bIsEnemy*/);
DECLARE_DELEGATE_TwoParams(FOnEnemyAction, ABLCombatSlot* /*Slot*/, ECombatActionType /*ActionType*/);

UCLASS()
class BLADEOFLEGEND_API ABLCombatSlot : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABLCombatSlot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	ABLCombatCharacter* GetCharacter() const
	{
		return Character;
	};
	uint32 GetIndex() const
	{
		return Index;
	};
	bool IsEnemy() const
	{
		return bIsEnemy;
	}

	float GetCooldown() const;

	void SpawnCharacter(const FCombatCharData& BaseData, const FAttackActionData& AttackData, const FDefendActionData& DefendData);
	void PauseCharCooldown();
	void UnPauseCharCooldown();

	void DoAction(ECombatActionType ActionType, ABLCombatSlot* TargetSlot);

private:
	UFUNCTION()
	void EndCharCooldown();
	UFUNCTION()
	void ActionEnded();

	void StartCharCooldown();

	UFUNCTION()
	void HandleCharDeath();
	UFUNCTION()
	void UpdateCharHealth();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BL|Combat")
	TObjectPtr<UBoxComponent> Box;

	
	UPROPERTY()
	bool bIsActive;
	UPROPERTY()
	bool bCanDoAction;

	FOnSelectedSlot OnSelectedSlot;
	FOnCharActionEnded OnCharActionEnded;
	FOnCharHealthUpdate OnCharHealthUpdate;
	FOnCharDeath OnCharDeath;
	FOnEnemyAction OnEnemyAction;

private:
	UPROPERTY()
	TObjectPtr<ABLCombatCharacter> Character;
	UPROPERTY(EditInstanceOnly, Category = "BL|Combat")
	uint32 Index;
	UPROPERTY(EditInstanceOnly, Category = "BL|Combat")
	bool bIsEnemy;
}; 
