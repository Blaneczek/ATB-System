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
	int32 ActionIndex;
	UPROPERTY()
	bool bEnemyAction;

	FActionQueue()
	{
		OwnerSlot = nullptr;
		TargetSlot = nullptr;
		ActionType = ECombatActionType::NONE;
		ActionIndex = 0;
		bEnemyAction = false;
	}

	FActionQueue(ABLCombatSlot* InOwnerSlot, ABLCombatSlot* InTargetSlot, ECombatActionType InActionType, int32 InIndex, bool InEnemyAction)
		:OwnerSlot(InOwnerSlot), TargetSlot(InTargetSlot), ActionType(InActionType), ActionIndex(InIndex), bEnemyAction(InEnemyAction)
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
	/** Creates heroes and sets data for gameplay and UI depending on Heroes DataAsset */
	void SetPlayerTeam();

	/** Creates enemies and sets data for gameplay and UI depending on Enemies DataAsset */
	void SetEnemyTeam();

	/** Initialize main combat widget */
	void InitializeWidget();

	/** Handles situations where one of the slots in the level has been clicked */
	void HandleSlotClicked(AActor* Slot);

	/** Highlights and sets as current clicked enemy slot */
	void ChooseEnemySlot(ABLCombatSlot* Slot);

	/** Highlights and sets as current clicked player slot */
	void ChoosePlayerSlot(ABLCombatSlot* Slot);

	/** Unhighlights and nulls current enemy slot */
	void ClearEnemySlot();
	/** Unhighlights given enemy slot */
	void ClearEnemySlot(ABLCombatSlot* EnemySlot);

	/** Unhighlights and nulls current player slot */
	void ClearPlayerSlot();

	/** Highlights and sets as current random available player slot */
	void ChooseRandomPlayerSlot();

	void AddActionToQueue(ABLCombatSlot* OwnerSlot, ABLCombatSlot* TargetSlot, ECombatActionType Action, int32 InActionIndex, bool bEnemyAction);

	/** Executes if it can the first action from the queue, checks the ability at constant intervals */
	void HandleActionsQueue();

	/** Pauses all cooldowns and calls the appropriate OwnerSlot function */
	void DoAction(ABLCombatSlot* OwnerSlot, ABLCombatSlot* TargetSlot, ECombatActionType Action, int32 InActionIndex, bool bEnemyAction);

	/** Finds new active target slot */
	ABLCombatSlot* FindNewTargetSlot(bool bEnemyAction);

	/** Chooses target for enemy action */
	void HandleEnemyAction(ABLCombatSlot* EnemySlot, ECombatActionType Action, int32 InActionIndex);

	/** Called after the completion of the currently executed action */
	void ActionEnded(ABLCombatSlot* OwnerSlot, bool bWasEnemy);

	/** Sets current ActionType and chosen action */
	void ChooseAction(ECombatActionType InActionType, int32 InActionIndex);

	/** Resets action and sets new CurrentPlayerSlot */
	void ResetAction(ABLCombatSlot* NewPlayerSlot);

	/** Player's actions */
	void PlayerAttackAction(ABLCombatSlot* EnemySlot);
	void PlayerDefendAction();

	/** Pauses the cooldowns of enemies and heroes */
	void PauseCooldowns();

	/** Unpauses the cooldowns of enemies and heroes */
	void UnPauseCooldowns();

	/** Shows actions of the selected hero */
	void ShowHeroActions(ABLCombatSlot* PlayerSlot);

	/** Hides actions of the selected  hero */
	void HideHeroActions(ABLCombatSlot* PlayerSlot);

	/** For combat widget */
	void UpdateHeroHealth(ABLCombatSlot* PlayerSlot);
	void UpdateHeroMagicEnergy(ABLCombatSlot* PlayerSlot);

	/** Handles character deaths */
	void CharacterDied(ABLCombatSlot* Slot, bool bIsEnemy);
	void HeroDied(ABLCombatSlot* PlayerSlot);
	void EnemyDied(ABLCombatSlot* EnemySlot);

	/** Checks if there are still active slots in enemy and player teams */
	bool CheckIfEndGame(bool& OutWonGame);

	void BindPlayerDelegetes();
	void BindEnemyDelegetes();

public:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "BL|Combat")
	TArray<TObjectPtr<ABLCombatSlot>> PlayerTeam;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "BL|Combat")
	TArray<TObjectPtr<ABLCombatSlot>> EnemyTeam;

	/** Data of every hero in Player team */
	UPROPERTY(EditDefaultsOnly, Category = "BL|Combat")
	TSoftObjectPtr<UBLHeroDataAsset> HeroesData;

	/** Data of every enemy in Enemy team */
	UPROPERTY(EditDefaultsOnly, Category = "BL|Combat")
	TSoftObjectPtr<UBLEnemyDataAsset> EnemiesData;

private:
	UPROPERTY()
	TObjectPtr<ABLCombatSlot> CurrentPlayerSlot;
	UPROPERTY()
	TObjectPtr<ABLCombatSlot> CurrentEnemySlot;

	UPROPERTY()
	TArray<FActionQueue> ActionQueue;

	/** Timer for queue function*/
	FTimerHandle ActionQueueTimer;

	UPROPERTY(EditInstanceOnly, Category = "BL|Combat")
	TSubclassOf<UBLCombatWidget> WidgetClass;
	UPROPERTY()
	TObjectPtr<UBLCombatWidget> Widget;


	/** Current selected action type */
	UPROPERTY()
	ECombatActionType ActionType;

	/** Current selected action index */
	UPROPERTY()
	int32 ActionIndex;

	/** If an action is currently being executed */
	UPROPERTY()
	bool bAction;
};
