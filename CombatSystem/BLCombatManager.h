// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BLCombatUtilities.h"
#include "BLWindowDisplayInterface.h"
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

		:OwnerSlot(InOwnerSlot), TargetsSlots(InTargetsSlots), ActionData(InActionData), bEnemyAction(InEnemyAction)
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
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** IBLWindowDisplay interface implementation 
	* Used when a combat is related to a quest, and the player must perform certain actions, after which the fight will end */	
	virtual void ShowQuestText_Implementation() override;

private:
	/** Creates heroes and sets data for gameplay and UI depending on Heroes DataAsset */
	void SetPlayerTeam();

	/** Creates enemies and sets data for gameplay and UI depending on Enemies DataAsset */
	void SetEnemyTeam();

	/** Initialize main combat widget */
	void InitializeWidget();

	/** Handles situations where one of the slots in the level has been clicked */
	void HandleSlotClicked(AActor* Slot);

	/** Deselects and clears the last clicked slot */
	void DeselectClickedSlot();

	/** Highlights and sets as current clicked target slot (could be hero or enemy) */
	void ChooseTargetSlot(ABLCombatSlot* Slot);

	/** Highlights and sets as current clicked player slot */
	void ChoosePlayerSlot(ABLCombatSlot* Slot);

	/** Highlights and sets as current clicked player slot */
	void ChoosePlayerSlot(int32 Index);

	/** Unhighlights and nulls current targets slots (could be heroes or enemies) */
	void ClearTargetsSlots();

	/** Unhighlights given target slot */
	void ClearTargetSlot(ABLCombatSlot* Slot);

	/** Unhighlights and nulls current player slot */
	void ClearPlayerSlot();

	/** Highlights and sets as current random available player slot */
	void ChooseRandomPlayerSlot();

	void AddActionToQueue(ABLCombatSlot* OwnerSlot, const TArray<ABLCombatSlot*>& TargetsSlots, const FCombatActionData& ActionData, bool bEnemyAction);

	/** Executes if it can the first action from the queue, checks the ability at constant intervals */
	void HandleActionsQueue();

	/** Pauses all cooldowns and calls the appropriate OwnerSlot function */
	void DoAction(ABLCombatSlot* OwnerSlot, const TArray<ABLCombatSlot*>& TargetsSlots, const FCombatActionData& ActionData, bool bEnemyAction);

	/** Finds new active target slot */
	ABLCombatSlot* FindNewTargetSlot(bool bEnemyAction);

	/** Chooses target for enemy action */
	void HandleEnemyAction(ABLCombatSlot* EnemySlot, const FCombatActionData& ActionData);

	/** Called after the completion of the currently executed action */
	void ActionEnded(ABLCombatSlot* OwnerSlot, bool bWasEnemy);

	/** Sets current ActionType and chosen action */
	void ChooseAction(const FCombatActionData& ActionData);

	/** Resets action and sets new CurrentPlayerSlot */
	void ResetAction(ABLCombatSlot* NewPlayerSlot);

	void PlayerAction();

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

	/** Handles characters deaths */
	void CharacterDied(ABLCombatSlot* Slot, bool bIsEnemy);
	void HeroDied(ABLCombatSlot* PlayerSlot);
	void EnemyDied(ABLCombatSlot* EnemySlot);

	/** Checks if there are still active slots in enemy and player teams */
	bool CheckIfEndGame(bool& OutWonGame);

	void HandleEndGame(bool bWonGame);

	void BindPlayerDelegetes();
	void BindEnemyDelegetes();

	void ExitCombat();

	void ShowDisplayWindow(const FText& InText, float Time);

	void SetHeroesCurrentHPME(UBLHeroDataAsset* HeroData);

public:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "BL|Combat")
	TArray<TObjectPtr<ABLCombatSlot>> PlayerTeam;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "BL|Combat")
	TArray<TObjectPtr<ABLCombatSlot>> EnemyTeam;

	/** Data of every hero in Player team */
	UPROPERTY(EditDefaultsOnly, Category = "BL|Combat")
	TSoftObjectPtr<UBLHeroDataAsset> HeroesData;

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
