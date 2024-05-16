// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BLCombatUtilities.h"
#include "Others/BLWindowDisplayInterface.h"
#include "BLCombatManager.generated.h"

class ABLCombatSlot;
class UBLHeroDataAsset;
class UBLEnemyDataAsset;
class UBLCombatWidget;
class UBLActionEntryData;
class UBLWinCombatWidget;
class UBLLostCombatWidget;

USTRUCT(BlueprintType)
struct FActionQueue
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<ABLCombatSlot> OwnerSlot;
	UPROPERTY()
	TArray<TObjectPtr<ABLCombatSlot>> TargetsSlots;
	UPROPERTY()
	FCombatActionData ActionData;
	UPROPERTY()
	bool bEnemyAction;

	FActionQueue()
		: OwnerSlot(nullptr), ActionData(FCombatActionData()), bEnemyAction(false)
	{}

	FActionQueue(ABLCombatSlot* InOwnerSlot, const TArray<ABLCombatSlot*>& InTargetsSlots, const FCombatActionData& InActionData, bool InEnemyAction)
		: OwnerSlot(InOwnerSlot), TargetsSlots(InTargetsSlots), ActionData(InActionData), bEnemyAction(InEnemyAction)
	{}
};

UCLASS()
class BLADEOFLEGEND_API ABLCombatManager : public AActor, public IBLWindowDisplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABLCombatManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	/** IBLWindowDisplay interface implementation. 
	/** Used when a combat is related to a quest, and the player must perform certain actions, after which the combat will end. */	
	virtual void ShowQuestText_Implementation() override;

private:
	/*******************/
	/*	    PLAYER	   */
	/*******************/

	/** Creates heroes and sets data for gameplay and UI. */
	void SetPlayerTeam();

	/** Handles situations where one of the slots in the level has been clicked. */
	void HandleSlotClicked(AActor* Slot);

	/** Highlights and sets as current clicked target slot (could be hero or enemy). */
	void ChooseTargetSlot(ABLCombatSlot* Slot);

	/** Highlights and sets as current clicked player slot. */
	void ChoosePlayerSlot(ABLCombatSlot* Slot);

	/** Highlights and sets as current clicked player slot. */
	void ChoosePlayerSlot(int32 Index);

	/** Unhighlights and nulls current targets slots (could be heroes or enemies). */
	void ClearTargetsSlots();

	/** Unhighlights given target slot. */
	void ClearTargetSlot(ABLCombatSlot* Slot);

	/** Unhighlights and nulls current player slot. */
	void ClearPlayerSlot();

	/** Highlights and sets as current available player slot. */
	void ChooseAvailablePlayerSlot();

	/** Deselects and clears the last clicked slot. */
	void DeselectClickedSlot();

	/** After selecting all targets, handles adding action to the queue and prepares slots for next use. */
	void ProcessPlayerAction();

	/** Sets current ActionType and chosen action. */
	void SetPlayerActionData(const FCombatActionData& ActionData);

	/** Resets action and sets new CurrentPlayerSlot */
	void ResetPlayerAction(ABLCombatSlot* NewPlayerSlot);

	/**
	 * Saves heroes' current hp after combat.
	 * @param MinPercentage: 0.0f - 1.f The minimum percentage of Max HP that will be after the combat. 
	 */
	void SetHeroesOverworldHP(float MinPercentage);

	void HeroDied(ABLCombatSlot* PlayerSlot);

	/** If successful, ends combat early. */
	void RunAway(bool bSuccessful);

	/********************************************/

	/*******************/
	/*	    ENEMY	   */
	/*******************/

	/** Creates enemies and sets data for gameplay and UI. */
	void SetEnemyTeam();

	/** Chooses target for enemy action */
	void HandleEnemyAction(ABLCombatSlot* EnemySlot, FCombatActionData&& ActionData);

	void EnemyDied(ABLCombatSlot* EnemySlot);

	/**********************************************/

	void BindInputDelegates();

	void BindSlotsDelegetes();

	void AddActionToQueue(ABLCombatSlot* OwnerSlot, const TArray<ABLCombatSlot*>& TargetsSlots, const FCombatActionData& ActionData, bool bEnemyAction);

	/** Executes if it can the first action from the queue, checks the ability at constant intervals */
	void HandleActionsQueue();

	/** Pauses all cooldowns and calls the appropriate OwnerSlot function */
	void DoAction(ABLCombatSlot* OwnerSlot, const TArray<ABLCombatSlot*>& TargetsSlots, const FCombatActionData& ActionData, bool bEnemyAction);

	/** Called after the completion of the currently executed action */
	void ActionEnded(ABLCombatSlot* OwnerSlot, bool bWasEnemy);

	/** Initializes main combat widget */
	void InitializeWidget();

	/** Finds new active target slot */
	ABLCombatSlot* FindNewTargetSlot(bool bEnemyAction);
	
	/** Pauses the cooldowns of enemies and heroes */
	void PauseCooldowns();

	/** Unpauses the cooldowns of enemies and heroes */
	void UnPauseCooldowns();

	/** Handles characters deaths */
	void CharacterDied(ABLCombatSlot* Slot, bool bIsEnemy);
	
	/** Checks if there are still active slots in enemy and player teams */
	bool CheckIfEndGame(bool& OutWonGame);

	void HandleEndGame(bool bWonGame);

	void ExitCombat();

public:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "BL|Combat")
	TArray<TObjectPtr<ABLCombatSlot>> PlayerTeam;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "BL|Combat")
	TArray<TObjectPtr<ABLCombatSlot>> EnemyTeam;

private:
	UPROPERTY()
	TObjectPtr<ABLCombatSlot> CurrentPlayerSlot;

	UPROPERTY()
	TObjectPtr<ABLCombatSlot> CurrentTargetSlot;

	UPROPERTY()
	TArray<TObjectPtr<ABLCombatSlot>> CurrentTargetsSlots;

	UPROPERTY()
	TArray<FActionQueue> ActionQueue;

	/** Timer for queue function*/
	FTimerHandle ActionQueueTimer;

	/** Main combat widget */
	UPROPERTY(EditDefaultsOnly, Category = "BL|Combat")
	TSubclassOf<UBLCombatWidget> WidgetClass;
	UPROPERTY()
	TObjectPtr<UBLCombatWidget> Widget;

	UPROPERTY(EditDefaultsOnly, Category = "BL|Combat")
	TSubclassOf<UUserWidget> LvlUPWidgetClass;

	/** Widget class when player won fight */
	UPROPERTY(EditDefaultsOnly, Category = "BL|Combat")
	TSubclassOf<UBLWinCombatWidget> WinWidgetClass;

	/** Widget class when player lost fight */
	UPROPERTY(EditDefaultsOnly, Category = "BL|Combat")
	TSubclassOf<UBLLostCombatWidget> LostWidgetClass;

	/** If an action is currently being executed */
	UPROPERTY()
	bool bAction;

	UPROPERTY()
	FCombatActionData CurrentActionData;

	UPROPERTY()
	ECombatActionType CurrentActionType;

	UPROPERTY()
	int32 QuestTextIndex;
};
