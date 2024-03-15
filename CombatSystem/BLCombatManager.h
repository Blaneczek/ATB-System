// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BLCombatUtilities.h"
#include "BLCombatManager.generated.h"

class ABLCombatSlot;
class UBLHeroDataAsset;
class UBLEnemyDataAsset;
class UBLCombatWidget;

USTRUCT(BlueprintType)
struct FActionQueue
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<ABLCombatSlot> OwnerSlot;
	UPROPERTY()
	TObjectPtr<ABLCombatSlot> TargetSlot;
	UPROPERTY()
	ECombatActionType ActionType;
	UPROPERTY()
	bool bEnemyAction;

	FActionQueue()
	{
		OwnerSlot = nullptr;
		TargetSlot = nullptr;
		ActionType = ECombatActionType::NONE;
		bEnemyAction = false;
	}

	FActionQueue(ABLCombatSlot* InOwnerSlot, ABLCombatSlot* InTargetSlot, ECombatActionType InActionType, bool InEnemyAction)
		:OwnerSlot(InOwnerSlot), TargetSlot(InTargetSlot), ActionType(InActionType), bEnemyAction(InEnemyAction)
	{}
};

UCLASS()
class BLADEOFLEGEND_API ABLCombatManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABLCombatManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void SetPlayerTeam();
	void SetEnemyTeam();

	void InitializeWidget();

	/** Slot related function*/
	void HandleSlotClicked(AActor* Slot);
	void ChooseEnemySlot(ABLCombatSlot* Slot);
	void ChoosePlayerSlot(ABLCombatSlot* Slot);
	void ClearEnemySlot();
	void ClearPlayerSlot();
	void ChooseRandomPlayerSlot();

	void AddActionToQueue(ABLCombatSlot* OwnerSlot, ABLCombatSlot* TargetSlot, ECombatActionType Action, bool bEnemyAction);
	void HandleActionsQueue();
	void DoAction(ABLCombatSlot* OwnerSlot, ABLCombatSlot* TargetSlot, ECombatActionType Action, bool bEnemyAction);
	ABLCombatSlot* FindNewTargetSlot(bool bEnemyAction);
	void HandleEnemyAction(ABLCombatSlot* EnemySlot, ECombatActionType Action);
	void ActionEnded(ABLCombatSlot* OwnerSlot, bool bWasEnemy);
	void ChooseAction(ECombatActionType InActionType, int32 ActionIndex);

	void PauseCooldowns();
	void UnPauseCooldowns();

	void ShowHeroActions(ABLCombatSlot* PlayerSlot);
	void HideHeroActions(ABLCombatSlot* PlayerSlot);
	void UpdateHeroHealth(ABLCombatSlot* PlayerSlot);

	void CharacterDied(ABLCombatSlot* Slot, bool bIsEnemy);
	void HeroDied(ABLCombatSlot* PlayerSlot);
	void EnemyDied(ABLCombatSlot* EnemySlot);
	bool CheckIfEndGame(bool& OutWonGame);

	void BindPlayerDelegetes();
	void BindEnemyDelegetes();
	

public:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "BL|Combat")
	TArray<TObjectPtr<ABLCombatSlot>> PlayerTeam;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "BL|Combat")
	TArray<TObjectPtr<ABLCombatSlot>> EnemyTeam;
	UPROPERTY(EditDefaultsOnly, Category = "BL|Combat")
	TSoftObjectPtr<UBLHeroDataAsset> HeroesData;
	UPROPERTY(EditDefaultsOnly, Category = "BL|Combat")
	TSoftObjectPtr<UBLEnemyDataAsset> EnemiesData;

private:
	UPROPERTY()
	TObjectPtr<ABLCombatSlot> CurrentPlayerSlot;
	UPROPERTY()
	TObjectPtr<ABLCombatSlot> CurrentEnemySlot;

	bool bAction;
	ECombatActionType ActionType;

	UPROPERTY()
	TArray<FActionQueue> ActionQueue;

	FTimerHandle ActionQueueTimer;

	UPROPERTY()
	TObjectPtr<UBLCombatWidget> Widget;
	UPROPERTY(EditInstanceOnly, Category = "BL|Combat")
	TSubclassOf<UBLCombatWidget> WidgetClass;
};
