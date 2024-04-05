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

// Sets default values
ABLCombatManager::ABLCombatManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bAction = false;
	ActionType = ECombatActionType::NONE;
	ActionIndex = 0;
	CurrentPlayerSlot = nullptr;
	CurrentTargetSlot = nullptr;
	ActionMECost = 0.f;
	ActionCrystalColor = ECrystalColor::NONE;
	ActionTargetsNum = 1;

	PlayerTeam.Init(nullptr, 5);
	EnemyTeam.Init(nullptr, 12);
}

// Called when the game starts or when spawned
void ABLCombatManager::BeginPlay()
{
	Super::BeginPlay();
	
	ABLCombatPlayerController* AC = Cast<ABLCombatPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (AC)
	{
		AC->OnSlotClicked.BindUObject(this, &ABLCombatManager::HandleSlotClicked);
		AC->OnSlotRemoved.BindUObject(this, &ABLCombatManager::DeselectClickedSlot);
	}

	InitializeWidget();
	SetPlayerTeam();
	SetEnemyTeam();
	BindPlayerDelegetes();
	BindEnemyDelegetes();
	if (Widget)
	{
		Widget->AddToViewport();
	}

	GetWorld()->GetTimerManager().SetTimer(ActionQueueTimer, this, &ABLCombatManager::HandleActionsQueue, 0.5f, true);

}

// Called every frame
void ABLCombatManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABLCombatManager::SetPlayerTeam()
{
	UBLHeroDataAsset* Data = HeroesData.LoadSynchronous();

	if (Data)
	{
		for (int32 Index = 0; Index < FMath::Clamp(Data->Heroes.Num(), 1, 5); ++Index)
		{
			const FCombatCharData CharBaseData = Data->CalculateBaseCombatData(Index);
			if (PlayerTeam[Index])
			{
				PlayerTeam[Index]->SpawnCharacter(CharBaseData, Data->Heroes[Index].AttackActions, Data->Heroes[Index].DefendActions, Data->Heroes[Index].CrystalActions, Data->Heroes[Index].SpecialActions);
				if (Widget)
				{
					Widget->AddHero(PlayerTeam[Index]->GetIndex(), CharBaseData);
					Widget->AddHeroActions(PlayerTeam[Index]->GetIndex(), CharBaseData, Data->Heroes[Index].AttackActions, Data->Heroes[Index].DefendActions, Data->Heroes[Index].CrystalActions, Data->Heroes[Index].SpecialActions);
				}
			}					
		};
	}

	HeroesData.Reset();
}

void ABLCombatManager::SetEnemyTeam()
{
	UBLGameInstance* GI = Cast<UBLGameInstance>(GetGameInstance());
	if (!GI)
	{
		return;
	}

	UBLEnemyDataAsset* Data = GI->EnemyData.LoadSynchronous();
	if (Data)
	{
		for (int32 Index = 0; Index < Data->Enemies.Num(); ++Index)
		{
			if (EnemyTeam[Index])
			{
				EnemyTeam[Index]->SpawnCharacter(Data->Enemies[Index].BaseData, Data->Enemies[Index].AttackActions, Data->Enemies[Index].DefendActions);
				if (Widget)
				{
					Widget->AddEnemy(EnemyTeam[Index]->GetIndex(), Data->Enemies[Index].BaseData.Name);
				}
			}
		}
	}

	GI->EnemyData.Reset();
}

void ABLCombatManager::InitializeWidget()
{
	if (WidgetClass)
	{
		Widget = CreateWidget<UBLCombatWidget>(GetWorld(), WidgetClass);
		Widget->OnActionChosen.BindUObject(this, &ABLCombatManager::ChooseAction);
		Widget->OnHeroSelected.BindUObject(this, &ABLCombatManager::ChoosePlayerSlot);
	}
}

void ABLCombatManager::HandleSlotClicked(AActor* Slot)
{
	ABLCombatSlot* CurrentSlot = Cast<ABLCombatSlot>(Slot);
	if (!CurrentSlot || !CurrentSlot->IsActive())
	{
		return;
	}

	switch (ActionType)
	{
		case ECombatActionType::ATTACK:
		{
			// If clicked on the enemy
			if (CurrentSlot->IsEnemy())
			{
				ChooseTargetSlot(CurrentSlot);
				PlayerAttackAction();
			}
			return;
		}
		case ECombatActionType::DEFEND:
		{
			// If clicked on the same hero
			if (CurrentSlot == CurrentPlayerSlot && CurrentSlot->bCanDoAction)
			{
				PlayerDefendAction();
			}
			return;
		}
		case ECombatActionType::CRYSTAL_SKILL:
		{
			ChooseTargetSlot(CurrentSlot);
			if (CurrentTargetsSlots.Num() >= ActionTargetsNum)
			{
				PlayerCrystalAction();
			}			
			return;
		}
		case ECombatActionType::SPECIAL_SKILL:
		{
			ChooseTargetSlot(CurrentSlot);
			if (CurrentTargetsSlots.Num() >= ActionTargetsNum)
			{
				PlayerSpecialAction();
			}
			return;
		}
		default: return;
	}	
}

void ABLCombatManager::DeselectClickedSlot()
{
	if (!CurrentTargetsSlots.IsEmpty())
	{
		ClearTargetSlot(CurrentTargetsSlots.Pop());
	}
}

void ABLCombatManager::ChooseTargetSlot(ABLCombatSlot* Slot)
{
	CurrentTargetsSlots.Add(Slot);
	Slot->SelectTarget(true);
}

void ABLCombatManager::ChoosePlayerSlot(ABLCombatSlot* Slot)
{
	CurrentPlayerSlot = Slot;
	ShowHeroActions(CurrentPlayerSlot);
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
		if (Slot)
		{		
			Slot->SelectTarget(false);
		}
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
		HideHeroActions(CurrentPlayerSlot);
		CurrentPlayerSlot->SelectHero(false);
		CurrentPlayerSlot = nullptr;
	}
}

void ABLCombatManager::ChooseRandomPlayerSlot()
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

void ABLCombatManager::AddActionToQueue(ABLCombatSlot* OwnerSlot, const TArray<ABLCombatSlot*>& TargetsSlots, ECombatActionType Action, int32 InActionIndex, bool bEnemyAction, ECrystalColor CrystalColor, UObject* InActionEntry)
{
	ActionQueue.Add(FActionQueue(OwnerSlot, TargetsSlots, Action, InActionIndex, bEnemyAction, CrystalColor, InActionEntry));
}

void ABLCombatManager::HandleActionsQueue()
{
	if (bAction || ActionQueue.IsEmpty())
	{
		return;
	}

	if (!ActionQueue[0].OwnerSlot || !ActionQueue[0].OwnerSlot->IsActive())
	{
		ActionQueue.RemoveAt(0);
		return;
	}
		
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

	DoAction(ActionQueue[0].OwnerSlot, ActionQueue[0].TargetsSlots, ActionQueue[0].ActionType, ActionQueue[0].ActionIndex, ActionQueue[0].bEnemyAction, ActionQueue[0].CrystalColor, ActionQueue[0].ActionEntry);
	ActionQueue.RemoveAt(0);
}

void ABLCombatManager::DoAction(ABLCombatSlot* OwnerSlot, TArray<ABLCombatSlot*> TargetsSlots, ECombatActionType Action, int32 InActionIndex, bool bEnemyAction, ECrystalColor CrystalColor, UObject* InActionEntry)
{	
	if (!OwnerSlot) return;

	bAction = true;
	PauseCooldowns();

	if (Widget)
	{
		Widget->SetIsActionVisibility(ESlateVisibility::Visible);
		Widget->PauseCooldownBars(true);
		if (!bEnemyAction)
		{
			Widget->ResetHeroCooldownBar(OwnerSlot->GetIndex());
		}
	}

	OwnerSlot->DoAction(Action, InActionIndex, TargetsSlots, CrystalColor, InActionEntry);
}

ABLCombatSlot* ABLCombatManager::FindNewTargetSlot(bool bEnemyAction)
{
	const TArray<ABLCombatSlot*>& Team = bEnemyAction ? PlayerTeam : EnemyTeam;;

	for (auto* Slot : Team)
	{
		if (Slot && Slot->IsActive())
		{
			return Slot;
		}
	}
		
	return nullptr;
}

void ABLCombatManager::HandleEnemyAction(ABLCombatSlot* EnemySlot, ECombatActionType Action, int32 InActionIndex)
{
	// now it gets random target. In the future maybe create more advanced way of choosing targets

	TArray<int32> ActiveSlots;

	for (int32 Index = 0; Index < PlayerTeam.Num(); ++Index)
	{
		if (PlayerTeam[Index] && PlayerTeam[Index]->IsActive())
		{
			ActiveSlots.Add(Index);
		}
	}

	if (!ActiveSlots.IsEmpty())
	{
		const int32 RandomIndex = FMath::RandRange(0, ActiveSlots.Num() - 1);
		
		TArray<ABLCombatSlot*> Targets;
		ABLCombatSlot* TargetSlot = PlayerTeam[ActiveSlots[RandomIndex]];
		if (TargetSlot)
		{
			Targets.Add(TargetSlot);
			AddActionToQueue(EnemySlot, Targets, Action, InActionIndex, true);
		}
	}
}

void ABLCombatManager::ActionEnded(ABLCombatSlot* OwnerSlot, bool bWasEnemy)
{
	if (!Widget)
	{
		return;
	}

	Widget->PauseCooldownBars(false);
	Widget->SetIsActionVisibility(ESlateVisibility::Hidden);
	if (!bWasEnemy)
	{
		Widget->StartHeroCooldownBar(OwnerSlot->GetIndex(), OwnerSlot->GetCooldown());
		UpdateHeroMagicEnergy(OwnerSlot);
	}

	UnPauseCooldowns();
	bAction = false;
}

void ABLCombatManager::ChooseAction(ECombatActionType InActionType, int32 InActionIndex, ECrystalColor CrystalColor, float MECost, int32 TargetsNum, UObject* InActionEntry)
{
	UE_LOG(LogTemp, Warning, TEXT("choosen action"));
	ActionType = InActionType;
	ActionIndex = InActionIndex;
	ActionCrystalColor = CrystalColor;
	ActionMECost = MECost;
	ActionTargetsNum = TargetsNum;
	ActionEntry = InActionEntry;
}

void ABLCombatManager::ResetAction(ABLCombatSlot* NewPlayerSlot)
{
	ActionType = ECombatActionType::NONE;
	ActionIndex = 0;
	ActionCrystalColor = ECrystalColor::NONE;
	ActionMECost = 0.f;
	ActionTargetsNum = 1;
	ActionEntry = nullptr;
	CurrentTargetsSlots.Empty();
	ClearPlayerSlot();
	ClearTargetsSlots();
	ChoosePlayerSlot(NewPlayerSlot);
}

void ABLCombatManager::PlayerAttackAction()
{
	AddActionToQueue(CurrentPlayerSlot, CurrentTargetsSlots, ActionType, ActionIndex, false);
	CurrentPlayerSlot->bCanDoAction = false;
	ActionType = ECombatActionType::NONE;
	if (Widget)
	{
		Widget->ResetHeroCooldownBar(CurrentPlayerSlot->GetIndex());
	}
	ClearPlayerSlot();
	ChooseRandomPlayerSlot();
	FTimerHandle ClearTargetDelay;
	FTimerDelegate ClearTargetDel;
	ClearTargetDel.BindUObject(this, &ABLCombatManager::ClearTargetsSlots);
	GetWorld()->GetTimerManager().SetTimer(ClearTargetDelay, ClearTargetDel, 1.f, false);
}

void ABLCombatManager::PlayerDefendAction()
{
	CurrentTargetsSlots.Add(CurrentPlayerSlot);
	AddActionToQueue(CurrentPlayerSlot, CurrentTargetsSlots, ActionType, ActionIndex, false);
	CurrentPlayerSlot->bCanDoAction = false;
	ActionType = ECombatActionType::NONE;
	if (Widget)
	{
		Widget->ResetHeroCooldownBar(CurrentPlayerSlot->GetIndex());
	}
	ClearPlayerSlot();
	ChooseRandomPlayerSlot();
}

void ABLCombatManager::PlayerCrystalAction()
{
	if (!Widget)
	{
		return;
	}

	if (ActionMECost > CurrentPlayerSlot->GetCharacter()->GetCurrentME())
	{
		Widget->ActivateNotEnoughME();
		return;
	}

	AddActionToQueue(CurrentPlayerSlot, CurrentTargetsSlots, ActionType, ActionIndex, false, ActionCrystalColor, ActionEntry);
	CurrentPlayerSlot->bCanDoAction = false;
	ActionType = ECombatActionType::NONE;

	Widget->ResetHeroCooldownBar(CurrentPlayerSlot->GetIndex());

	ClearPlayerSlot();
	ChooseRandomPlayerSlot();

	FTimerHandle ClearTargetDelay;
	FTimerDelegate ClearTargetDel;
	ClearTargetDel.BindUObject(this, &ABLCombatManager::ClearTargetsSlots);
	GetWorld()->GetTimerManager().SetTimer(ClearTargetDelay, ClearTargetDel, 1.f, false);
}

void ABLCombatManager::PlayerSpecialAction()
{
	if (!Widget)
	{
		return;
	}

	if (ActionMECost > CurrentPlayerSlot->GetCharacter()->GetCurrentME())
	{
		Widget->ActivateNotEnoughME();
		return;
	}

	AddActionToQueue(CurrentPlayerSlot, CurrentTargetsSlots, ActionType, ActionIndex, false, ECrystalColor::NONE, ActionEntry);
	CurrentPlayerSlot->bCanDoAction = false;
	ActionType = ECombatActionType::NONE;

	Widget->ResetHeroCooldownBar(CurrentPlayerSlot->GetIndex());

	ClearPlayerSlot();
	ChooseRandomPlayerSlot();

	FTimerHandle ClearTargetDelay;
	FTimerDelegate ClearTargetDel;
	ClearTargetDel.BindUObject(this, &ABLCombatManager::ClearTargetsSlots);
	GetWorld()->GetTimerManager().SetTimer(ClearTargetDelay, ClearTargetDel, 1.f, false);
}

void ABLCombatManager::PauseCooldowns()
{
	for (ABLCombatSlot* Slot : PlayerTeam)
	{
		if (Slot && Slot->IsActive())
		{
			Slot->PauseCharCooldown();
		}
	}
	for (ABLCombatSlot* Slot : EnemyTeam)
	{
		if (Slot && Slot->IsActive())
		{
			Slot->PauseCharCooldown();
		}
	}
}

void ABLCombatManager::UnPauseCooldowns()
{
	for (ABLCombatSlot* Slot : PlayerTeam)
	{
		if (Slot && Slot->IsActive())
		{
			Slot->UnPauseCharCooldown();
		}
	}
	for (ABLCombatSlot* Slot : EnemyTeam)
	{
		if (Slot && Slot->IsActive())
		{
			Slot->UnPauseCharCooldown();
		}
	}
}

void ABLCombatManager::ShowHeroActions(ABLCombatSlot* PlayerSlot)
{
	if (Widget)
	{
		Widget->SetCurrentHero(PlayerSlot->GetIndex());
	}
}

void ABLCombatManager::HideHeroActions(ABLCombatSlot* PlayerSlot)
{
	if (Widget)
	{
		Widget->ResetCurrentHero(PlayerSlot->GetIndex());
	}
}

void ABLCombatManager::UpdateHeroHealth(ABLCombatSlot* PlayerSlot)
{
	if (Widget)
	{
		Widget->UpdateHeroHealth(PlayerSlot->GetIndex(), PlayerSlot->GetCharacter()->GetMaxHP(), PlayerSlot->GetCharacter()->GetCurrentHP());
	}
}

void ABLCombatManager::UpdateHeroMagicEnergy(ABLCombatSlot* PlayerSlot)
{
	if (Widget)
	{
		Widget->UpdateHeroMagicEnergy(PlayerSlot->GetIndex(), PlayerSlot->GetCharacter()->GetMaxME(), PlayerSlot->GetCharacter()->GetCurrentME());
	}
}

void ABLCombatManager::CharacterDied(ABLCombatSlot* Slot, bool bIsEnemy)
{
	if (bIsEnemy)
	{
		EnemyDied(Slot);
	}
	else
	{
		HeroDied(Slot);
	}

	bool bWonGame = false;
	if (CheckIfEndGame(bWonGame))
	{
		//TODO: HandleEndGame(bWonGame);
	}
}

void ABLCombatManager::HeroDied(ABLCombatSlot* PlayerSlot)
{
	if (PlayerSlot == CurrentPlayerSlot)
	{
		ClearPlayerSlot();
		ChooseRandomPlayerSlot();
	}

	if (Widget)
	{
		Widget->HeroDied(PlayerSlot->GetIndex());
	}
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

bool ABLCombatManager::CheckIfEndGame(bool& OutWonGame)
{
	uint8 Checker = 0;
	OutWonGame = false;

	for (ABLCombatSlot* Slot : PlayerTeam)
	{
		if (Slot && Slot->IsActive())
		{
			Checker++;
			OutWonGame = true;
			break;
		}
	}

	for (ABLCombatSlot* Slot : EnemyTeam)
	{
		if (Slot && Slot->IsActive())
		{
			Checker++;
			break;
		}
	}

	return Checker == 2 ? false : true;
}

void ABLCombatManager::BindPlayerDelegetes()
{
	for (ABLCombatSlot* Slot : PlayerTeam)
	{
		if (Slot && Slot->IsActive())
		{
			Slot->OnCharActionEnded.BindUObject(this, &ABLCombatManager::ActionEnded);
			Slot->OnSelectedSlot.BindLambda([this](ABLCombatSlot* Slot) { if (!CurrentPlayerSlot) ChoosePlayerSlot(Slot); });
			Slot->OnCharHealthUpdate.BindUObject(this, &ABLCombatManager::UpdateHeroHealth);
			Slot->OnCharDeath.BindUObject(this, &ABLCombatManager::CharacterDied);
		}
	}
}

void ABLCombatManager::BindEnemyDelegetes()
{
	for (ABLCombatSlot* Slot : EnemyTeam)
	{
		if (Slot && Slot->IsActive())
		{
			Slot->OnEnemyAction.BindUObject(this, &ABLCombatManager::HandleEnemyAction);
			Slot->OnCharActionEnded.BindUObject(this, &ABLCombatManager::ActionEnded);
			Slot->OnCharDeath.BindUObject(this, &ABLCombatManager::CharacterDied);
		}
	}
}


