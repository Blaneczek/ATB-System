// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLCombatManager.h"
#include "BLCombatPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "BLCombatSlot.h"
#include "Characters/BLCombatCharacter.h"
#include "Kismet/KismetArrayLibrary.h"
#include "Data/BLHeroDataAsset.h"
#include "Data/BLEnemyDataAsset.h"
#include "UObject/SoftObjectPtr.h"
#include "UI/BLCombatWidget.h"
#include "UI/BLActionsWidget.h"
#include "BladeOfLegend/AREK/GameInstance/BLGameInstance.h"
#include "UI/BLWinCombatWidget.h"
#include "UI/BLLostCombatWidget.h"
#include "UI/BLLevelUPWidget.h"

// Sets default values
ABLCombatManager::ABLCombatManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bAction = false;
	CurrentActionType = ECombatActionType::NONE;
	CurrentActionData = FCombatActionData();
	CurrentPlayerSlot = nullptr;
	CurrentTargetSlot = nullptr;
	QuestTextIndex = 0;

	// Max size of teams
	PlayerTeam.Init(nullptr, 5);
	EnemyTeam.Init(nullptr, 12);
}

// Called when the game starts or when spawned
void ABLCombatManager::BeginPlay()
{
	Super::BeginPlay();

	BindInputDelegates();
	InitializeWidget();
	SetPlayerTeam();
	SetEnemyTeam();
	BindSlotsDelegetes();

	if (Widget)
	{
		Widget->AddToViewport();
	}

	GetWorld()->GetTimerManager().SetTimer(ActionQueueTimer, this, &ABLCombatManager::HandleActionsQueue, 0.5f, true);

	ShowQuestText_Implementation();
}

/**********       PLAYER       ***********/

void ABLCombatManager::SetPlayerTeam()
{
	UBLGameInstance* GI = Cast<UBLGameInstance>(GetGameInstance());	
	if (!GI)
	{
		return;
	}

	for (int32 Index = 0; Index < FMath::Clamp(GI->SaveGameData.HeroesData.Heroes.Num(), 0, 5); ++Index)
	{
		const FCombatCharData& CharBaseData = GI->CalculateBaseCombatData(Index);
		if (PlayerTeam[Index] && Widget)
		{
			PlayerTeam[Index]->SpawnHero(CharBaseData, GI->SaveGameData.HeroesData.Heroes[Index].CombatActions, GI->CombatData.bSneakAttack);
			GI->CombatData.bSneakAttack ? Widget->AddHero(PlayerTeam[Index]->GetIndex(), CharBaseData, true) : Widget->AddHero(PlayerTeam[Index]->GetIndex(), CharBaseData, false);
			Widget->AddHeroActions(PlayerTeam[Index]->GetIndex(), CharBaseData, GI->SaveGameData.HeroesData.Heroes[Index].CombatActions, GI->CombatData.bCanRunAway);
		}
	}
}

void ABLCombatManager::HandleSlotClicked(AActor* Slot)
{
	ABLCombatSlot* CurrentSlot = Cast<ABLCombatSlot>(Slot);
	if (!CurrentSlot || !CurrentSlot->IsActive())
	{
		return;
	}

	// If the player doesn't have enough ME for an action, display the widget.
	if (CurrentActionType != ECombatActionType::NONE && CurrentActionData.MECost > CurrentPlayerSlot->GetCharacter()->GetCurrentME())
	{
		Widget->ActivateNotEnoughME();
		return;
	}

	switch (CurrentActionData.Flow) 
	{
		case ECombatActionFlow::DEFAULT:
		{
			if (CurrentSlot == CurrentPlayerSlot)
			{
				ChooseTargetSlot(CurrentSlot);
				break;
			}
			return;
		}
		case ECombatActionFlow::COLUMN_MELEE:
		{
			if (CurrentSlot->IsEnemy())
			{
				int32 IndexStart = 0;
				int32 IndexEnd = 0;
				// 1. column
				if (CurrentSlot->GetIndex() <= 3)
				{
					IndexStart = 0;
					IndexEnd = 3;
				}
				// 2. column
				else if (CurrentSlot->GetIndex() >= 4 && CurrentSlot->GetIndex() <= 7)
				{
					IndexStart = 4;
					IndexEnd = 7;
				}
				// 3. column
				else
				{
					IndexStart = 8;
					IndexEnd = 11;
				}

				for (IndexStart; IndexStart <= IndexEnd; ++IndexStart)
				{
					if (EnemyTeam[IndexStart] && EnemyTeam[IndexStart]->IsActive())
					{
						ChooseTargetSlot(EnemyTeam[IndexStart]);
					}
				}
				break;
			}
			return;				
		}
		case ECombatActionFlow::BOUNCE_RANGE:
		{
			if (CurrentSlot->IsEnemy())
			{
				ChooseTargetSlot(CurrentSlot);
				if (CurrentActionData.TargetsNum == 1)
				{
					break;
				}

				// Projectiles cannot bounce 2 times in a row on the same target
				int32 DisabledIndex = CurrentSlot->GetIndex();
				for (int32 Index = 0; Index < CurrentActionData.TargetsNum - 1; ++Index)
				{
					TArray<int32> AvailableIndexes;
					for (const auto& EnemySlot : EnemyTeam)
					{
						if (EnemySlot && EnemySlot->IsActive() && EnemySlot->GetIndex() != DisabledIndex)
						{
							AvailableIndexes.Add(EnemySlot->GetIndex());
						}
					}

					if (AvailableIndexes.IsEmpty())
					{
						break; 
					}

					const int32 RandomIndex = FMath::RandRange(0, AvailableIndexes.Num() - 1);						
					CurrentTargetsSlots.Add(EnemyTeam[AvailableIndexes[RandomIndex]]);
					DisabledIndex = AvailableIndexes[RandomIndex];
				}
				break;
			}
			return;
		}
		case ECombatActionFlow::WHOLE_TEAM_IN_PLACE:
		{
			if (CurrentSlot->IsEnemy())
			{	
				for (const auto& EnemySlot : EnemyTeam)
				{
					if (EnemySlot && EnemySlot->IsActive())
					{
						ChooseTargetSlot(EnemySlot);
					}
				}
				break;
			}
			return;
		}
		default:
		{
			if (CurrentSlot->IsEnemy())
			{
				ChooseTargetSlot(CurrentSlot);
				if (CurrentTargetsSlots.Num() >= CurrentActionData.TargetsNum)
				{
					break;
				}
			}
			return;
		}
	}

	ProcessPlayerAction();
}

void ABLCombatManager::ChooseTargetSlot(ABLCombatSlot* Slot)
{
	if (Slot)
	{		
		CurrentTargetsSlots.Add(Slot);
		Slot->SelectTarget(true);
	}
}

void ABLCombatManager::ChoosePlayerSlot(ABLCombatSlot* Slot)
{
	CurrentPlayerSlot = Slot;
	if (Widget)
	{
		Widget->SetCurrentHero(Slot->GetIndex());
	}
	CurrentPlayerSlot->SelectHero(true);
}

void ABLCombatManager::ChoosePlayerSlot(int32 Index)
{
	if (PlayerTeam.IsValidIndex(Index))
	{
		ClearPlayerSlot();
		ChoosePlayerSlot(PlayerTeam[Index]);
	}
}

void ABLCombatManager::ClearTargetsSlots()
{
	for (ABLCombatSlot* Slot : CurrentTargetsSlots)
	{		
		ClearTargetSlot(Slot);
	}

	CurrentTargetsSlots.Empty();
}

void ABLCombatManager::ClearTargetSlot(ABLCombatSlot* Slot)
{
	if (Slot)
	{
		Slot->SelectTarget(false);
	}
}

void ABLCombatManager::ClearPlayerSlot()
{
	if (CurrentPlayerSlot)
	{	
		if (Widget)
		{
			Widget->ResetCurrentHero(CurrentPlayerSlot->GetIndex());
		}
		CurrentPlayerSlot->SelectHero(false);
		CurrentPlayerSlot = nullptr;
	}
}

void ABLCombatManager::ChooseAvailablePlayerSlot()
{
	for (ABLCombatSlot* Slot : PlayerTeam)
	{
		if (Slot && Slot->bCanDoAction)
		{
			ChoosePlayerSlot(Slot);
			return;
		}
	}
}

void ABLCombatManager::DeselectClickedSlot()
{
	if (!CurrentTargetsSlots.IsEmpty())
	{
		ClearTargetSlot(CurrentTargetsSlots.Pop());
	}
}

void ABLCombatManager::ProcessPlayerAction()
{
	if (!Widget)
	{
		return;
	}

	AddActionToQueue(CurrentPlayerSlot, CurrentTargetsSlots, CurrentActionData, false);
	CurrentPlayerSlot->bCanDoAction = false;
	CurrentActionType = ECombatActionType::NONE;

	Widget->ResetHeroCooldownBar(CurrentPlayerSlot->GetIndex());

	ClearPlayerSlot();
	ChooseAvailablePlayerSlot();

	FTimerHandle ClearTargetDelay;
	FTimerDelegate ClearTargetDel;
	ClearTargetDel.BindUObject(this, &ABLCombatManager::ClearTargetsSlots);
	GetWorld()->GetTimerManager().SetTimer(ClearTargetDelay, ClearTargetDel, 1.f, false);
}

void ABLCombatManager::SetPlayerActionData(const FCombatActionData& ActionData)
{
	CurrentActionType = ActionData.Type;
	CurrentActionData = ActionData;

	// If it is DEFEND, do it without choosing target
	if (CurrentActionType == ECombatActionType::DEFEND && CurrentPlayerSlot)
	{
		ChooseTargetSlot(CurrentPlayerSlot);
		ProcessPlayerAction();
	}

	//If it is RUN AWAY, do it without choosing target and try to escape combat (30% chance)
	if (CurrentActionType == ECombatActionType::RUN_AWAY && CurrentPlayerSlot)
	{
		ChooseTargetSlot(CurrentPlayerSlot);
		ProcessPlayerAction();
	}
}

void ABLCombatManager::ResetPlayerAction(ABLCombatSlot* NewPlayerSlot)
{
	CurrentActionType = ECombatActionType::NONE;
	ClearPlayerSlot();
	ClearTargetsSlots();
	ChoosePlayerSlot(NewPlayerSlot);
}

void ABLCombatManager::SetHeroesOverworldHP(float MinPercentage)
{
	UBLGameInstance* GI = Cast<UBLGameInstance>(GetGameInstance());
	if (!GI)
	{
		return;
	}

	for (int32 Index = 0; Index < GI->SaveGameData.HeroesData.Heroes.Num(); ++Index)
	{
		if (PlayerTeam.IsValidIndex(Index) && PlayerTeam[Index] && PlayerTeam[Index]->GetCharacter())
		{
			GI->SaveGameData.HeroesData.Heroes[Index].HeroAttributes.CurrentHP = PlayerTeam[Index]->GetCharacter()->GetCurrentHP() > PlayerTeam[Index]->GetCharacter()->GetMaxHP() * MinPercentage
																				? PlayerTeam[Index]->GetCharacter()->GetCurrentHP() : PlayerTeam[Index]->GetCharacter()->GetMaxHP() * MinPercentage;
		}
	}
}

void ABLCombatManager::HeroDied(ABLCombatSlot* PlayerSlot)
{
	if (Widget)
	{
		PlayerSlot == CurrentPlayerSlot ? Widget->HeroDied(PlayerSlot->GetIndex(), true) : Widget->HeroDied(PlayerSlot->GetIndex(), false);
	}

	if (PlayerSlot == CurrentPlayerSlot)
	{
		ClearPlayerSlot();
		ChooseAvailablePlayerSlot();
	}
}

void ABLCombatManager::RunAway(bool bSuccessful)
{
	if (!bSuccessful)
	{
		if (Widget)
		{
			Widget->ShowRunAwayText(false);
		}
		return;
	}

	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

	if (Widget)
	{
		Widget->ShowRunAwayText(true);
	}

	UBLGameInstance* GI = Cast<UBLGameInstance>(GetGameInstance());
	if (GI)
	{
		if (GI->GetEnemies().Contains(GI->CombatData.EnemyTag))
		{
			// Remove enemy from overworld
			GI->SetEnemyFlag(GI->CombatData.EnemyTag, true);
		}

		SetHeroesOverworldHP(0.3f);
		FTimerDelegate DelayDel;
		DelayDel.BindUObject(this, &ABLCombatManager::ExitCombat);
		FTimerHandle DelayTimer;
		GetWorld()->GetTimerManager().SetTimer(DelayTimer, DelayDel, 2.f, false);
	}
}
/****************************************/

/**********       ENEMY       ***********/

void ABLCombatManager::SetEnemyTeam()
{
	UBLGameInstance* GI = Cast<UBLGameInstance>(GetGameInstance());
	if (!GI)
	{
		return;
	}

	UBLEnemyDataAsset* Data = GI->CombatData.EnemyData;
	if (!Data)
	{
		return;
	}

	for (int32 Index = 0; Index < Data->Enemies.Num(); ++Index)
	{
		if (EnemyTeam[Index] && Widget)
		{
			EnemyTeam[Index]->SpawnEnemy(Data->Enemies[Index].BaseData, Data->Enemies[Index].Level, Data->Enemies[Index].Actions, GI->CombatData.bSneakAttack);
			Widget->AddEnemy(EnemyTeam[Index]->GetIndex(), Data->Enemies[Index].BaseData.Name, Data->Enemies[Index].Level, Data->Enemies[Index].BaseData.Cooldown);
		}
	}	
}

void ABLCombatManager::HandleEnemyAction(ABLCombatSlot* EnemySlot, FCombatActionData&& ActionData)
{
	TArray<int32> ActiveSlots;
	for (const auto& Slot : PlayerTeam)
	{
		if (Slot && Slot->IsActive())
		{
			ActiveSlots.Add(Slot->GetIndex());
		}
	}

	if (ActiveSlots.IsEmpty())
	{
		return;
	}

	TArray<ABLCombatSlot*> Targets;

	switch (ActionData.Flow)
	{
		case ECombatActionFlow::DEFAULT:
		{
			Targets.Add(EnemySlot);
			break;
		}
		case ECombatActionFlow::DEFAULT_MELEE:
		case ECombatActionFlow::DEFAULT_RANGE:
		case ECombatActionFlow::MULTIPLE_MELEE:
		case ECombatActionFlow::MULTIPLE_RANGE:
		{
			for (int32 Index = 0; Index < ActionData.TargetsNum; ++Index)
			{
				const int32 RandomIndex = FMath::RandRange(0, ActiveSlots.Num() - 1);
				Targets.Add(PlayerTeam[ActiveSlots[RandomIndex]]);
			}
			break;
		}
		case ECombatActionFlow::BOUNCE_RANGE:
		{
			int32 RandomIndex = FMath::RandRange(0, ActiveSlots.Num() - 1);
			Targets.Add(PlayerTeam[ActiveSlots[RandomIndex]]);
			int32 DisabledIndex = ActiveSlots[RandomIndex];

			for (int32 Index = 0; Index < ActionData.TargetsNum - 1; ++Index)
			{
				TArray<int32> AvailableIndexes;
				for (const auto& ActiveIndex : ActiveSlots)
				{
					if (ActiveIndex != DisabledIndex)
					{
						AvailableIndexes.Add(ActiveIndex);
					}
				}
				if (AvailableIndexes.IsEmpty())
				{
					break;
				}
				RandomIndex = FMath::RandRange(0, AvailableIndexes.Num() - 1);
				Targets.Add(PlayerTeam[AvailableIndexes[RandomIndex]]);
				DisabledIndex = AvailableIndexes[RandomIndex];
			}
			break;
		}
		case ECombatActionFlow::SUMMON_ALLIES:
		{
			TArray<int32> AvailableSlotsIndex;
			for (const auto& Slot : EnemyTeam)
			{
				// not active because we are summoning in free slots
				if (Slot && !Slot->IsActive() && Slot != EnemySlot)
				{
					AvailableSlotsIndex.Add(Slot->GetIndex());
				}
			}

			if (AvailableSlotsIndex.IsEmpty())
			{
				Targets.Add(FindNewTargetSlot(true));
				ActionData = FCombatActionData(ECombatActionType::ATTACK, ECombatActionFlow::DEFAULT_MELEE, 0);
				break;
			}

			for (int32 Index = 0; Index < ActionData.TargetsNum; ++Index)
			{
				if (AvailableSlotsIndex.IsEmpty())
				{
					break;
				}
				const int32 RandomIndex = FMath::RandRange(0, AvailableSlotsIndex.Num() - 1);
				Targets.Add(EnemyTeam[AvailableSlotsIndex[RandomIndex]]);
				AvailableSlotsIndex.RemoveAt(RandomIndex);
			}
			break;
		}
		case ECombatActionFlow::KILL_ALLIES:
		{
			TArray<int32> AvailableSlotsIndex;
			for (auto& Slot : EnemyTeam)
			{
				if (Slot && Slot->IsActive() && Slot != EnemySlot)
				{
					AvailableSlotsIndex.Add(Slot->GetIndex());
				}
			}

			if (AvailableSlotsIndex.IsEmpty())
			{
				Targets.Add(FindNewTargetSlot(true));
				ActionData = FCombatActionData(ECombatActionType::ATTACK, ECombatActionFlow::DEFAULT_MELEE, 0);
				break;
			}

			const int32 Random = FMath::RandRange(1, ActionData.TargetsNum);
			for (int32 Index = 0; Index < Random; ++Index)
			{
				if (AvailableSlotsIndex.IsEmpty())
				{
					break;
				}
				const int32 RandomIndex = FMath::RandRange(0, AvailableSlotsIndex.Num() - 1);
				Targets.Add(EnemyTeam[AvailableSlotsIndex[RandomIndex]]);
				AvailableSlotsIndex.RemoveAt(RandomIndex);
			}
			break;
		}
		case ECombatActionFlow::WHOLE_TEAM_IN_PLACE:
		{
			for (const auto& Slot : PlayerTeam)
			{
				if (Slot && Slot->IsActive())
				{
					Targets.Add(Slot);
				}
			}
			break;
		}
		default: break;
	}

	AddActionToQueue(EnemySlot, Targets, ActionData, true);
}

void ABLCombatManager::EnemyDied(ABLCombatSlot* EnemySlot)
{
	if (CurrentTargetsSlots.Contains(EnemySlot))
	{
		ClearTargetSlot(EnemySlot);
	}

	if (Widget)
	{
		Widget->EnemyDied(EnemySlot->GetIndex());
	}
}

/*************************************************************/

void ABLCombatManager::BindInputDelegates()
{
	ABLCombatPlayerController* AC = Cast<ABLCombatPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (AC)
	{
		AC->OnSlotClicked.BindUObject(this, &ABLCombatManager::HandleSlotClicked);
		AC->OnSlotRemoved.BindUObject(this, &ABLCombatManager::DeselectClickedSlot);
	}
}

void ABLCombatManager::BindSlotsDelegetes()
{
	for (const auto& Slot : PlayerTeam)
	{
		if (Slot && Slot->IsActive())
		{
			Slot->OnCharActionEnded.BindUObject(this, &ABLCombatManager::ActionEnded);
			Slot->OnSelectedSlot.BindWeakLambda(this, [this, Slot](ABLCombatSlot* InSlot) { if (!CurrentPlayerSlot) ChoosePlayerSlot(Slot); });
			Slot->OnCharHealthUpdate.BindWeakLambda(this, [this, Slot]() { if (Widget) Widget->UpdateHeroHealth(Slot->GetIndex(), Slot->GetCharacter()->GetMaxHP(), Slot->GetCharacter()->GetCurrentHP()); });
			Slot->OnCharDeath.BindUObject(this, &ABLCombatManager::CharacterDied);
			Slot->OnEscapeCombat.BindUObject(this, &ABLCombatManager::RunAway);
		}
	}

	for (const auto& Slot : EnemyTeam)
	{
		if (Slot)
		{
			Slot->OnEnemyAction.BindUObject(this, &ABLCombatManager::HandleEnemyAction);
			Slot->OnCharActionEnded.BindUObject(this, &ABLCombatManager::ActionEnded);
			Slot->OnCharDeath.BindUObject(this, &ABLCombatManager::CharacterDied);
			Slot->OnCharHealthUpdate.BindWeakLambda(this, [this, Slot]() { if (Widget) Widget->UpdateEnemyHealth(Slot->GetIndex(), Slot->GetCharacter()->GetCurrentHP(), Slot->GetCharacter()->GetMaxHP()); });
			Slot->OnCharSpawned.BindWeakLambda(this, [this](int32 Index, const FString& Name, int32 Level, float Cooldown) { if (Widget) Widget->AddEnemy(Index, Name, Level, Cooldown); });
			Slot->OnCharDestroyed.BindWeakLambda(this, [this](int32 Index) { if (Widget) Widget->RemoveEnemy(Index); });
		}
	}
}

void ABLCombatManager::AddActionToQueue(ABLCombatSlot* OwnerSlot, const TArray<ABLCombatSlot*>& TargetsSlots, const FCombatActionData& ActionData, bool bEnemyAction)
{
	ActionQueue.Add(FActionQueue(OwnerSlot, TargetsSlots, ActionData, bEnemyAction));
}

void ABLCombatManager::HandleActionsQueue()
{
	// Do nothing if other action is currently performed. 
	if (bAction || ActionQueue.IsEmpty())
	{
		return;
	}

	// Delete action from queue if owner is not active (dead).
	if (!ActionQueue[0].OwnerSlot || !ActionQueue[0].OwnerSlot->IsActive())
	{
		ActionQueue.RemoveAt(0);
		bAction = false;
		return;
	}
		
	// If it's not summon allies type of action, check if all targets are active (alive). If not, find new active target.
	if (ActionQueue[0].ActionData.Flow != ECombatActionFlow::SUMMON_ALLIES)
	{
		for (int32 Index = ActionQueue[0].TargetsSlots.Num() - 1; Index >= 0; --Index)
		{
			if (ActionQueue[0].TargetsSlots[Index] && !ActionQueue[0].TargetsSlots[Index]->IsActive())
			{
				ABLCombatSlot* NewTargetSlot = FindNewTargetSlot(ActionQueue[0].bEnemyAction);
				if (NewTargetSlot)
				{
					ActionQueue[0].TargetsSlots[Index] = NewTargetSlot;
				}
				else
				{
					ActionQueue[0].TargetsSlots.RemoveSingle(ActionQueue[0].TargetsSlots[Index]);
				}
	
			}
		}
	}

	DoAction(ActionQueue[0].OwnerSlot, ActionQueue[0].TargetsSlots, ActionQueue[0].ActionData, ActionQueue[0].bEnemyAction);
	ActionQueue.RemoveAt(0);
}

void ABLCombatManager::DoAction(ABLCombatSlot* OwnerSlot, const TArray<ABLCombatSlot*>& TargetsSlots, const FCombatActionData& ActionData, bool bEnemyAction)
{	
	if (!OwnerSlot)
	{
		bAction = false;
		return;
	}

	bAction = true;
	PauseCooldowns();

	if (Widget)
	{
		Widget->PauseCooldownBars(true);
		Widget->ShowActionTextDisplay(true);
	}

	OwnerSlot->DoAction(TargetsSlots, ActionData, this);
}

void ABLCombatManager::ActionEnded(ABLCombatSlot* OwnerSlot, bool bWasEnemy)
{
	UnPauseCooldowns();
	bAction = false;

	if (!Widget)
	{
		return;
	}

	Widget->PauseCooldownBars(false);
	Widget->ShowActionTextDisplay(false);

	if (!bWasEnemy)
	{
		Widget->StartHeroCooldownBar(OwnerSlot->GetIndex(), OwnerSlot->GetCooldown());
		Widget->UpdateHeroMagicEnergy(OwnerSlot->GetIndex(), OwnerSlot->GetCharacter()->GetMaxME(), OwnerSlot->GetCharacter()->GetCurrentME());	
	}
}

void ABLCombatManager::InitializeWidget()
{
	if (WidgetClass)
	{
		Widget = CreateWidget<UBLCombatWidget>(GetWorld(), WidgetClass);
		Widget->OnActionChosen.BindUObject(this, &ABLCombatManager::SetPlayerActionData);
		Widget->OnHeroSelected.BindUObject(this, &ABLCombatManager::ChoosePlayerSlot);
		Widget->OnResetCurrentActionType.BindWeakLambda(this, [this]() {CurrentActionType = ECombatActionType::NONE; });
	}
}

ABLCombatSlot* ABLCombatManager::FindNewTargetSlot(bool bEnemyAction)
{
	const TArray<ABLCombatSlot*>& Team = bEnemyAction ? PlayerTeam : EnemyTeam;;

	for (const auto& Slot : Team)
	{
		if (Slot && Slot->IsActive())
		{
			return Slot;
		}
	}
		
	return nullptr;
}

void ABLCombatManager::PauseCooldowns()
{
	for (const auto& Slot : PlayerTeam)
	{
		if (Slot && Slot->IsActive())
		{
			Slot->PauseCharCooldown();
		}
	}
	for (const auto& Slot : EnemyTeam)
	{
		if (Slot && Slot->IsActive())
		{
			Slot->PauseCharCooldown();
		}
	}
}

void ABLCombatManager::UnPauseCooldowns()
{
	for (const auto& Slot : PlayerTeam)
	{
		if (Slot && Slot->IsActive())
		{
			Slot->UnPauseCharCooldown();
		}
	}
	for (const auto& Slot : EnemyTeam)
	{
		if (Slot && Slot->IsActive())
		{
			Slot->UnPauseCharCooldown();
		}
	}
}

void ABLCombatManager::CharacterDied(ABLCombatSlot* Slot, bool bIsEnemy)
{
	bIsEnemy ? EnemyDied(Slot) : HeroDied(Slot);

	bool bWonGame = false;
	if (CheckIfEndGame(bWonGame))
	{
		HandleEndGame(bWonGame);
	}
}

bool ABLCombatManager::CheckIfEndGame(bool& OutWonGame)
{
	uint8 Checker = 0;
	OutWonGame = false;

	for (const auto& Slot : PlayerTeam)
	{
		if (Slot && Slot->IsActive())
		{
			Checker++;
			OutWonGame = true;
			break;
		}
	}

	for (const auto& Slot : EnemyTeam)
	{
		if (Slot && Slot->IsActive())
		{
			Checker++;
			break;
		}
	}

	return Checker == 2 ? false : true;
}

void ABLCombatManager::HandleEndGame(bool bWonGame)
{
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

	UBLGameInstance* GI = Cast<UBLGameInstance>(GetGameInstance());
	if (!GI)
	{
		return;
	}

	if (bWonGame)
	{	
		if (GI->GetEnemies().Contains(GI->CombatData.EnemyTag))
		{
			// Enemy is dead.
			GI->SetEnemyFlag(GI->CombatData.EnemyTag, true);
		}

		{
			SetHeroesOverworldHP(0.3f);
			GI->AddCombatReward(GI->PostCombatData.Experience, GI->PostCombatData.Gold, GI->PostCombatData.Items);

			if (WinWidgetClass)
			{		
				UBLWinCombatWidget* WinWidget = CreateWidget<UBLWinCombatWidget>(GetWorld(), WinWidgetClass);
				WinWidget->SetData(GI->PostCombatData.Experience, GI->PostCombatData.Gold, GI->PostCombatData.Items);
				WinWidget->OnWinGame.BindUObject(this, &ABLCombatManager::ExitCombat);
				WinWidget->AddToViewport();
			}

			if (LvlUPWidgetClass && GI->IsLevelUP())
			{
				UBLLevelUPWidget* LvlUPWidget = CreateWidget<UBLLevelUPWidget>(GetWorld(), LvlUPWidgetClass);
				LvlUPWidget->SetData(GI->GetLevelUPData());
				LvlUPWidget->AddToViewport();
			}
		}		
	}
	else
	{
		if (LostWidgetClass)
		{
			UBLLostCombatWidget* LostWidget = CreateWidget<UBLLostCombatWidget>(GetWorld(), LostWidgetClass);
			LostWidget->OnLostGame.BindWeakLambda(this, [this, GI]() 
				{ 
					GI->SetUseCombatPlayerLocation(false);
					GI->LoadGame(); 
				});
			LostWidget->AddToViewport();
		}
	}
}

void ABLCombatManager::ExitCombat()
{
	UBLGameInstance* GI = Cast<UBLGameInstance>(GetGameInstance());
	if (!GI)
	{
		return;
	}

	if (Widget)
	{
		Widget->RemoveFromParent();
	}

	// Camera fade before opening new level
	APlayerCameraManager* CM = Cast<APlayerCameraManager>(UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0));
	if (CM)
	{
		CM->StartCameraFade(0.f, 1.f, 1.f, FLinearColor(0.f, 0.f, 0.f), false, true);
		FTimerDelegate DelayDel;
		DelayDel.BindWeakLambda(this, [this, GI]() { UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), GI->GetPostCombatMapName()); });
		FTimerHandle DelayTimer;
		GetWorld()->GetTimerManager().SetTimer(DelayTimer, DelayDel, 1.5f, false);
	}
	else
	{
		UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), GI->GetPostCombatMapName());
	}
}

void ABLCombatManager::ShowQuestText_Implementation()
{
	UBLGameInstance* GI = Cast<UBLGameInstance>(GetGameInstance());
	if (GI && GI->CombatData.QuestDisplayTexts.IsValidIndex(QuestTextIndex))
	{
		const float ShowTime = QuestTextIndex == 0 ? 0.f : 4.f;
		if (Widget)
		{
			Widget->ShowWindowText(GI->CombatData.QuestDisplayTexts[QuestTextIndex], ShowTime);
		}
		QuestTextIndex++;
		if (QuestTextIndex == GI->CombatData.QuestDisplayTexts.Num())
		{
			HandleEndGame(true);
		}
	}
}


