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

// Sets default values
ABLCombatManager::ABLCombatManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bAction = false;
	ActionType = ECombatActionType::NONE;
	CurrentPlayerSlot = nullptr;
	CurrentEnemySlot = nullptr;

	for (int32 Index = 0; Index < 5; ++Index)
	{
		PlayerTeam.Add(nullptr);
	}
	for (int32 Index = 0; Index < 12; ++Index)
	{
		EnemyTeam.Add(nullptr);
	}
}

// Called when the game starts or when spawned
void ABLCombatManager::BeginPlay()
{
	Super::BeginPlay();
	
	ABLCombatPlayerController* AC = Cast<ABLCombatPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (AC)
	{
		AC->OnSlotClicked.BindUObject(this, &ABLCombatManager::HandleSlotClicked);
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
		for (int32 Index = 0; Index < Data->Heroes.Num(); ++Index)
		{
			const FCombatCharData CharBaseData = Data->CalculateBaseCombatData(Index);
			if (PlayerTeam[Index])
			{
				PlayerTeam[Index]->SpawnCharacter(CharBaseData, Data->Heroes[Index].AttackActions, Data->Heroes[Index].DefendActions);
				if (Widget)
				{
					Widget->AddHero(PlayerTeam[Index]->GetIndex(), CharBaseData);
					Widget->AddHeroActions(PlayerTeam[Index]->GetIndex(), CharBaseData, Data->Heroes[Index].AttackActions, Data->Heroes[Index].DefendActions);
				}
			}					
		};
	}

	HeroesData.Reset();
}

void ABLCombatManager::SetEnemyTeam()
{
	UBLEnemyDataAsset* Data = EnemiesData.LoadSynchronous();

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

	EnemiesData.Reset();
}

void ABLCombatManager::InitializeWidget()
{
	if (WidgetClass)
	{
		Widget = CreateWidget<UBLCombatWidget>(GetWorld(), WidgetClass);
		Widget->OnActionChosen.BindUObject(this, &ABLCombatManager::ChooseAction);
	}
}

void ABLCombatManager::HandleSlotClicked(AActor* Slot)
{
	ABLCombatSlot* CurrentSlot = Cast<ABLCombatSlot>(Slot);
	if (!CurrentSlot || !CurrentSlot->bIsActive)
	{
		return;
	}

	switch (ActionType)
	{
		case ECombatActionType::NONE:
		{
			// If clicked on the hero
			if (!CurrentSlot->IsEnemy() && CurrentSlot->bCanDoAction)
			{
				ClearPlayerSlot();
				ChoosePlayerSlot(CurrentSlot);
			}
			return;
		}
		case ECombatActionType::ATTACK:
		{
			// If clicked on the enemy
			if (CurrentSlot->IsEnemy())
			{
				PlayerAttackAction(CurrentSlot);
			}
			// If clicked on the hero
			else if (CurrentSlot->bCanDoAction)
			{
				ResetAction(CurrentSlot);
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
			// If clicked on a different hero
			else if (!CurrentSlot->IsEnemy() && CurrentSlot->bCanDoAction)
			{
				ResetAction(CurrentSlot);
			}
			return;
		}
		default: return;
	}	
}

void ABLCombatManager::ChooseEnemySlot(ABLCombatSlot* Slot)
{
	CurrentEnemySlot = Slot;
	//TODO: change clicked effect
	if (CurrentEnemySlot->ClickedMaterial)
	{
		CurrentEnemySlot->Platform->SetMaterial(0, CurrentEnemySlot->ClickedMaterial);
	}
	CurrentEnemySlot->bClicked = true;
}

void ABLCombatManager::ChoosePlayerSlot(ABLCombatSlot* Slot)
{
	CurrentPlayerSlot = Slot;
	ShowHeroActions(CurrentPlayerSlot);
	//TODO: change clicked effect
	if (CurrentPlayerSlot->ClickedMaterial)
	{
		CurrentPlayerSlot->Platform->SetMaterial(0, CurrentPlayerSlot->ClickedMaterial);
	}
	CurrentPlayerSlot->bClicked = true;
}

void ABLCombatManager::ClearEnemySlot()
{
	if (CurrentEnemySlot)
	{
		//TODO: change clicked effect
		if (CurrentEnemySlot->DefaultMaterial)
		{
			CurrentEnemySlot->Platform->SetMaterial(0, CurrentEnemySlot->DefaultMaterial);
		}
		CurrentEnemySlot->bClicked = false;
		CurrentEnemySlot = nullptr;
	}
}

void ABLCombatManager::ClearEnemySlot(ABLCombatSlot* EnemySlot)
{
	if (EnemySlot)
	{
		if (EnemySlot->DefaultMaterial)
		{
			EnemySlot->Platform->SetMaterial(0, EnemySlot->DefaultMaterial);
		}
		EnemySlot->bClicked = false;
	}
}

void ABLCombatManager::ClearPlayerSlot()
{
	if (CurrentPlayerSlot)
	{	
		HideHeroActions(CurrentPlayerSlot);
		//TODO: change clicked effect
		if (CurrentPlayerSlot->DefaultMaterial)
		{
			CurrentPlayerSlot->Platform->SetMaterial(0, CurrentPlayerSlot->DefaultMaterial);
		}
		CurrentPlayerSlot->bClicked = false;
		HideHeroActions(CurrentPlayerSlot);
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

void ABLCombatManager::AddActionToQueue(ABLCombatSlot* OwnerSlot, ABLCombatSlot* TargetSlot, ECombatActionType Action, int32 InActionIndex, bool bEnemyAction)
{
	ActionQueue.Add(FActionQueue(OwnerSlot, TargetSlot, Action, InActionIndex, bEnemyAction));
}

void ABLCombatManager::HandleActionsQueue()
{
	if (bAction || ActionQueue.IsEmpty()) return;

	if (ActionQueue[0].OwnerSlot && ActionQueue[0].TargetSlot)
	{
		if (!ActionQueue[0].OwnerSlot->bIsActive)
		{
			ActionQueue.RemoveAt(0);
			return;
		}

		if (ActionQueue[0].TargetSlot->bIsActive)
		{
			DoAction(ActionQueue[0].OwnerSlot, ActionQueue[0].TargetSlot, ActionQueue[0].ActionType, ActionQueue[0].ActionIndex, ActionQueue[0].bEnemyAction);
			ActionQueue.RemoveAt(0);
			return;
		}
		else
		{
			ABLCombatSlot* NewTargetSlot = FindNewTargetSlot(ActionQueue[0].bEnemyAction);
			if (NewTargetSlot)
			{
				DoAction(ActionQueue[0].OwnerSlot, NewTargetSlot, ActionQueue[0].ActionType, ActionQueue[0].ActionIndex, ActionQueue[0].bEnemyAction);
				ActionQueue.RemoveAt(0);
				return;
			}
			else
			{
				ActionQueue.RemoveAt(0);
				return;
			}
		}
	}
	else
	{
		ActionQueue.RemoveAt(0);
		return;
	}
}

void ABLCombatManager::DoAction(ABLCombatSlot* OwnerSlot, ABLCombatSlot* TargetSlot, ECombatActionType Action, int32 InActionIndex, bool bEnemyAction)
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

	OwnerSlot->DoAction(Action, InActionIndex, TargetSlot);
}

ABLCombatSlot* ABLCombatManager::FindNewTargetSlot(bool bEnemyAction)
{
	if (bEnemyAction)
	{
		for (ABLCombatSlot* Slot : PlayerTeam)
		{
			if (Slot && Slot->bIsActive)
			{
				return Slot;
			}
		}
		return nullptr;
	}
	else
	{
		for (ABLCombatSlot* Slot : EnemyTeam)
		{
			if (Slot && Slot->bIsActive)
			{
				return Slot;
			}
		}
		return nullptr;
	}
}

void ABLCombatManager::HandleEnemyAction(ABLCombatSlot* EnemySlot, ECombatActionType Action, int32 InActionIndex)
{
	// now it gets random target. In the future maybe create more advanced way of choosing targets

	TArray<int32> ActiveSlots;

	for (int32 Index = 0; Index < PlayerTeam.Num(); ++Index)
	{
		if (PlayerTeam[Index] && PlayerTeam[Index]->bIsActive)
		{
			ActiveSlots.Add(Index);
		}
	}

	if (!ActiveSlots.IsEmpty())
	{
		const int32 RandomIndex = FMath::RandRange(0, ActiveSlots.Num() - 1);
		
		ABLCombatSlot* TargetSlot = PlayerTeam[ActiveSlots[RandomIndex]];
		if (TargetSlot)
		{
			AddActionToQueue(EnemySlot, TargetSlot, Action, InActionIndex, true);
		}
	}
}

void ABLCombatManager::ActionEnded(ABLCombatSlot* OwnerSlot, bool bWasEnemy)
{
	if (Widget)
	{
		Widget->PauseCooldownBars(false);
		Widget->SetIsActionVisibility(ESlateVisibility::Hidden);
		if (!bWasEnemy)
		{
			Widget->StartHeroCooldownBar(OwnerSlot->GetIndex(), OwnerSlot->GetCooldown());
		}
	}

	UnPauseCooldowns();
	bAction = false;
}

void ABLCombatManager::ChooseAction(ECombatActionType InActionType, int32 InActionIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("choosen action"));
	ActionType = InActionType;
	ActionIndex = InActionIndex;
}

void ABLCombatManager::ResetAction(ABLCombatSlot* NewPlayerSlot)
{
	ActionType = ECombatActionType::NONE;
	ClearPlayerSlot();
	ClearEnemySlot();
	ChoosePlayerSlot(NewPlayerSlot);
}

void ABLCombatManager::PlayerAttackAction(ABLCombatSlot* EnemySlot)
{
	ClearEnemySlot();
	ChooseEnemySlot(EnemySlot);
	AddActionToQueue(CurrentPlayerSlot, CurrentEnemySlot, ActionType, ActionIndex, false);
	CurrentPlayerSlot->bCanDoAction = false;
	ActionType = ECombatActionType::NONE;
	if (Widget)
	{
		Widget->ResetHeroCooldownBar(CurrentPlayerSlot->GetIndex());
	}
	ClearPlayerSlot();
	ChooseRandomPlayerSlot();
	FTimerHandle ClearEnemyDelay;
	FTimerDelegate ClearEnemyDel;
	ClearEnemyDel.BindUObject(this, &ABLCombatManager::ClearEnemySlot, EnemySlot);
	GetWorld()->GetTimerManager().SetTimer(ClearEnemyDelay, ClearEnemyDel, 1.f, false);
}

void ABLCombatManager::PlayerDefendAction()
{
	AddActionToQueue(CurrentPlayerSlot, CurrentPlayerSlot, ActionType, ActionIndex, false);
	CurrentPlayerSlot->bCanDoAction = false;
	ActionType = ECombatActionType::NONE;
	if (Widget)
	{
		Widget->ResetHeroCooldownBar(CurrentPlayerSlot->GetIndex());
	}
	ClearPlayerSlot();
	ChooseRandomPlayerSlot();
}

void ABLCombatManager::PauseCooldowns()
{
	for (ABLCombatSlot* Slot : PlayerTeam)
	{
		if (Slot && Slot->bIsActive)
		{
			Slot->PauseCharCooldown();
		}
	}
	for (ABLCombatSlot* Slot : EnemyTeam)
	{
		if (Slot && Slot->bIsActive)
		{
			Slot->PauseCharCooldown();
		}
	}
}

void ABLCombatManager::UnPauseCooldowns()
{
	for (ABLCombatSlot* Slot : PlayerTeam)
	{
		if (Slot && Slot->bIsActive)
		{
			Slot->UnPauseCharCooldown();
		}
	}
	for (ABLCombatSlot* Slot : EnemyTeam)
	{
		if (Slot && Slot->bIsActive)
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
	if (EnemySlot == CurrentEnemySlot)
	{
		ClearEnemySlot();
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
		if (Slot && Slot->bIsActive)
		{
			Checker++;
			OutWonGame = true;
			break;
		}
	}
	for (ABLCombatSlot* Slot : EnemyTeam)
	{
		if (Slot && Slot->bIsActive)
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
		if (Slot && Slot->bIsActive)
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
		if (Slot && Slot->bIsActive)
		{
			Slot->OnEnemyAction.BindUObject(this, &ABLCombatManager::HandleEnemyAction);
			Slot->OnCharActionEnded.BindUObject(this, &ABLCombatManager::ActionEnded);
			Slot->OnCharDeath.BindUObject(this, &ABLCombatManager::CharacterDied);
		}
	}
}


