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

// Sets default values
ABLCombatManager::ABLCombatManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bAction = false;
	CurrentActionType = ECombatActionType::NONE;
	CurrentActionData = FCombatActionData();
	CurrentPlayerSlot = nullptr;
	CurrentTargetSlot = nullptr;

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
	if (!Data)
	{
		return;
	}

	for (int32 Index = 0; Index < FMath::Clamp(Data->Heroes.Num(), 1, 5); ++Index)
	{
		const FCombatCharData CharBaseData = Data->CalculateBaseCombatData(Index);
		if (PlayerTeam[Index] && Widget)
		{
			PlayerTeam[Index]->SpawnCharacter(CharBaseData, Data->Heroes[Index].CombatActions);
			Widget->AddHero(PlayerTeam[Index]->GetIndex(), CharBaseData);
			Widget->AddHeroActions(PlayerTeam[Index]->GetIndex(), CharBaseData, Data->Heroes[Index].CombatActions);
		}
	}
}

void ABLCombatManager::SetEnemyTeam()
{
	UBLGameInstance* GI = Cast<UBLGameInstance>(GetGameInstance());
	if (!GI)
	{
		return;
	}

	UBLEnemyDataAsset* Data = GI->CombatData.EnemyData.LoadSynchronous();
	if (!Data)
	{
		return;
	}

	for (int32 Index = 0; Index < Data->Enemies.Num(); ++Index)
	{
		if (EnemyTeam[Index] && Widget)
		{
			EnemyTeam[Index]->SpawnCharacter(Data->Enemies[Index].BaseData, Data->Enemies[Index].AttackActions, Data->Enemies[Index].DefendActions);
			Widget->AddEnemy(EnemyTeam[Index]->GetIndex(), Data->Enemies[Index].BaseData.Name);
		}
	}	
}

void ABLCombatManager::InitializeWidget()
{
	if (WidgetClass)
	{
		Widget = CreateWidget<UBLCombatWidget>(GetWorld(), WidgetClass);
		Widget->OnActionChosen.BindUObject(this, &ABLCombatManager::ChooseAction);
		Widget->OnHeroSelected.BindUObject(this, &ABLCombatManager::ChoosePlayerSlot);
		Widget->OnResetCurrentActionType.BindLambda([this]() {CurrentActionType = ECombatActionType::NONE; });
	}
}

void ABLCombatManager::HandleSlotClicked(AActor* Slot)
{
	ABLCombatSlot* CurrentSlot = Cast<ABLCombatSlot>(Slot);
	if (!CurrentSlot || !CurrentSlot->IsActive())
	{
		return;
	}

	switch (CurrentActionType)
	{
		case ECombatActionType::ATTACK:
		{
			if (CurrentSlot->IsEnemy())
			{
				ChooseTargetSlot(CurrentSlot);
				break;
			}
			return;
		}
		case ECombatActionType::DEFEND:
		{
			// If clicked on the same hero
			if (CurrentSlot == CurrentPlayerSlot)
			{
				ChooseTargetSlot(CurrentSlot);
				break;
			}
			return;
		}
		case ECombatActionType::CRYSTAL_SKILL:
		case ECombatActionType::SPECIAL_SKILL:
		case ECombatActionType::ITEM:
		{
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
						if (CurrentSlot->GetIndex() <= 3)
						{
							IndexStart = 0;
							IndexEnd = 3;
						}
						else if (CurrentSlot->GetIndex() >= 4 && CurrentSlot->GetIndex() <= 7)
						{
							IndexStart = 4;
							IndexEnd = 7;
						}
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
				default:
				{
					ChooseTargetSlot(CurrentSlot);
					if (CurrentTargetsSlots.Num() >= CurrentActionData.TargetsNum)
					{
						break;
					}
					return;
				}
			}
			break;
		}
		default: return;
	}

	PlayerAction();
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
	if (Slot)
	{		
		CurrentTargetsSlots.Add(Slot);
		Slot->SelectTarget(true);
	}
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

void ABLCombatManager::AddActionToQueue(ABLCombatSlot* OwnerSlot, const TArray<ABLCombatSlot*>& TargetsSlots, const FCombatActionData& ActionData, bool bEnemyAction)
{
	ActionQueue.Add(FActionQueue(OwnerSlot, TargetsSlots, ActionData, bEnemyAction));
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

	DoAction(ActionQueue[0].OwnerSlot, ActionQueue[0].TargetsSlots, ActionQueue[0].ActionData, ActionQueue[0].bEnemyAction);
	ActionQueue.RemoveAt(0);
}

void ABLCombatManager::DoAction(ABLCombatSlot* OwnerSlot, const TArray<ABLCombatSlot*>& TargetsSlots, const FCombatActionData& ActionData, bool bEnemyAction)
{	
	if (!OwnerSlot)
	{
		return;
	}

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

	OwnerSlot->DoAction(TargetsSlots, ActionData);
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

void ABLCombatManager::HandleEnemyAction(ABLCombatSlot* EnemySlot, const FCombatActionData& ActionData)
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
			AddActionToQueue(EnemySlot, Targets, ActionData, true);
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

void ABLCombatManager::ChooseAction(const FCombatActionData& ActionData)
{
	UE_LOG(LogTemp, Warning, TEXT("choosen action"));
	CurrentActionType = ActionData.Type;
	CurrentActionData = ActionData;
}

void ABLCombatManager::ResetAction(ABLCombatSlot* NewPlayerSlot)
{
	CurrentActionType = ECombatActionType::NONE;
	ClearPlayerSlot();
	ClearTargetsSlots();
	ChoosePlayerSlot(NewPlayerSlot);
}

void ABLCombatManager::PlayerAction()
{
	if (!Widget)
	{
		return;
	}

	if (CurrentActionData.MECost > CurrentPlayerSlot->GetCharacter()->GetCurrentME())
	{
		Widget->ActivateNotEnoughME();
		return;
	}
	AddActionToQueue(CurrentPlayerSlot, CurrentTargetsSlots, CurrentActionData, false);
	CurrentPlayerSlot->bCanDoAction = false;
	CurrentActionType = ECombatActionType::NONE;

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
		UE_LOG(LogTemp, Warning, TEXT("END"));
		HandleEndGame(bWonGame);
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

void ABLCombatManager::HandleEndGame(bool bWonGame)
{
	UBLGameInstance* GI = Cast<UBLGameInstance>(GetGameInstance());
	if (!GI)
	{
		return;
	}

	if (bWonGame)
	{		
		UBLHeroDataAsset* HeroData = HeroesData.LoadSynchronous();
		if (!HeroData)
		{
			//TODO: save post combat data
		}

		if (WinWidgetClass)
		{		
			UBLWinCombatWidget* WinWidget = CreateWidget<UBLWinCombatWidget>(GetWorld(), WinWidgetClass);
			WinWidget->SetData(GI->PostCombatData.Experience, GI->PostCombatData.Money);
			WinWidget->OnEndGame.BindUObject(this, &ABLCombatManager::ExitCombat);
			WinWidget->AddToViewport();
		}		
	}
	else
	{
		if (LostWidgetClass)
		{
			UBLLostCombatWidget* LostWidget = CreateWidget<UBLLostCombatWidget>(GetWorld(), LostWidgetClass);
			//LostWidget->OnEndGame.BindUObject(this, &ABLCombatManager::ExitCombat);
			LostWidget->AddToViewport();
		}
	}
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

void ABLCombatManager::ExitCombat()
{
	UBLGameInstance* GI = Cast<UBLGameInstance>(GetGameInstance());
	if (GI)
	{
		UGameplayStatics::OpenLevel(GetWorld(), GI->PostCombatData.LevelName);
	}
}


