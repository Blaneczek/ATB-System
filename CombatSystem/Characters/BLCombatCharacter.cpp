// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLCombatCharacter.h"
#include "PaperFlipbookComponent.h"
#include "PaperZDAnimationComponent.h"
#include "PaperZDAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "BLRangeProjectile.h"
#include "Navigation/PathFollowingComponent.h"
#include "Actions/BLAction.h"
#include "Components/WidgetComponent.h"
#include "UI/Entries/BLActionEntryData.h"
#include "UI/Entries/BLItemEntryData.h"
#include "BladeOfLegend/DAWID/Items/BLCombatItem.h"
#include "Kismet/KismetMathLibrary.h"
#include "BLCombatSlot.h"

ABLCombatCharacter::ABLCombatCharacter()
{
	DMGDisplay = CreateDefaultSubobject<UWidgetComponent>(TEXT("DMG Display"));
	DMGDisplay->SetupAttachment(PaperFlipbook);

	StatusDisplay = CreateDefaultSubobject<UWidgetComponent>(TEXT("Bleeding Display"));
	StatusDisplay->SetupAttachment(PaperFlipbook);

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bDead = false;
	AIC = nullptr;
	CurrentAction = nullptr;
	Movement->MaxWalkSpeed = 900.f;
	ProjectileTargetsNum = 0;
	ProjectileTargetIndex = 0;
}

void ABLCombatCharacter::BeginPlay()
{
	Super::BeginPlay();

	AIC = Cast<AAIController>(GetController());
}

void ABLCombatCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABLCombatCharacter::SetData(const FCombatCharData& InBaseData, const FCombatActions& InCombatActions, const FTransform& InSlotTransform)
{
	SetData(InBaseData, InCombatActions.AttackActions);

	DefendAction = InCombatActions.DefendAction;
	CrystalActions = InCombatActions.CrystalActions;
	SpecialActions = InCombatActions.SpecialActions;
	ItemActions = InCombatActions.ItemActions;
	RunAwayAction = InCombatActions.RunAwayAction;

	SlotTransform = InSlotTransform;
}

void ABLCombatCharacter::SetData(const FCombatCharData& InBaseData, const TArray<TSoftClassPtr<UBLAction>>& InActions)
{
	BaseData = InBaseData;
	CurrentHP = BaseData.CurrentHP;
	CurrentME = BaseData.MaxME;
	CurrentDefense = BaseData.BaseDefense;
	CurrentCooldown = BaseData.Cooldown;
	CurrentDodge = BaseData.BaseDodge;
	CurrentAttackDMG = BaseData.BaseAttackDMG;

	AttackActions = InActions;

	PaperFlipbook->SetFlipbook(BaseData.Sprite);

	// PURE VIRTUAL FUNCTION CALL BUG
	//GetAnimationComponent()->SetAnimInstanceClass(BaseData.AnimInstanceClass);
}


void ABLCombatCharacter::CreateAction(const FVector& OwnerSlotLocation, const TArray<ABLCombatCharacter*>& Targets, const FCombatActionData& ActionData, AActor* CombatManager)
{
	ClickedActionEntry = Cast<UBLActionEntryData>(ActionData.ActionEntry);

	switch (ActionData.Type)
	{
		case ECombatActionType::ATTACK:
		{
			if (!AttackActions.IsValidIndex(ActionData.Index))
			{							
				break;
			}
			SlotLocation = OwnerSlotLocation;
			TargetCharacters = Targets;

			CurrentAction = NewObject<UBLAction>(this, AttackActions[ActionData.Index].LoadSynchronous());
			if (CurrentAction)
			{
				CurrentAction->OnCreateAction(this, CombatManager);
			}
			else
			{
				break;
			}
			return;
		}
		case ECombatActionType::DEFEND:
		{
			CurrentAction = NewObject<UBLAction>(this, DefendAction.LoadSynchronous());
			if (CurrentAction)
			{
				CurrentAction->OnCreateAction(this, CombatManager);
			}
			else
			{
				break;
			}
			return;
		}
		case ECombatActionType::CRYSTAL_SKILL:
		{
			if (ActionData.CrystalColor != ECrystalColor::NONE && !CrystalActions.Find(ActionData.CrystalColor)->Skills.IsValidIndex(ActionData.Index))
			{
				break;
			}

			SlotLocation = OwnerSlotLocation;
			TargetCharacters = Targets;

			CurrentAction = NewObject<UBLAction>(this, CrystalActions.Find(ActionData.CrystalColor)->Skills[ActionData.Index].LoadSynchronous());
			if (CurrentAction)
			{
				CurrentAction->OnCreateAction(this, CombatManager);
			}
			else
			{
				break;
			}
			return;
		}
		case ECombatActionType::SPECIAL_SKILL:
		{
			if (!SpecialActions.IsValidIndex(ActionData.Index))
			{
				break;
			}

			SlotLocation = OwnerSlotLocation;
			TargetCharacters = Targets;

			CurrentAction = NewObject<UBLAction>(this, SpecialActions[ActionData.Index].LoadSynchronous());
			if (CurrentAction)
			{
				CurrentAction->OnCreateAction(this, CombatManager);
			}
			else
			{
				break;
			}
			return;
		}
		case ECombatActionType::ITEM:
		{
			if (!ItemActions.IsValidIndex(ActionData.Index))
			{
				break;
			}

			SlotLocation = OwnerSlotLocation;
			TargetCharacters = Targets;

			// Deleting used item
			UBLItemEntryData* ItemEntry = Cast<UBLItemEntryData>(ActionData.ActionEntry);
			if (ItemEntry)
			{
				ItemEntry->OnDeleteFromList.ExecuteIfBound(ItemEntry->Index);
			}
			else
			{
				break;
			}

			UBLCombatItem* Item = Cast<UBLCombatItem>(ItemActions[ActionData.Index].LoadSynchronous()->GetDefaultObject());
			if (!Item) break;

			CurrentAction = NewObject<UBLAction>(this, Item->Action);
			if (CurrentAction)
			{
				CurrentAction->OnCreateAction(this, CombatManager);
			}
			else
			{
				break;
			}
			return;
		}
		case ECombatActionType::RUN_AWAY:
		{
			// 30% chance for escape
			const int32 Random = FMath::RandRange(1, 100);
			if (Random <= 30)
			{
				OnEscape.ExecuteIfBound(true);
			}
			else
			{
				OnEscape.ExecuteIfBound(false);
				EndAction(true);
			}
			return;
		}
	}

	// if something went wrong, end Action
	EndAction(true);
}


void ABLCombatCharacter::CreateAction(const FVector& OwnerSlotLocation, const TArray<ABLCombatSlot*>& Targets, const FCombatActionData& ActionData, AActor* CombatManager)
{
	ClickedActionEntry = Cast<UBLActionEntryData>(ActionData.ActionEntry);
	switch (ActionData.Type)
	{
		case ECombatActionType::ATTACK:
		{
			if (!AttackActions.IsValidIndex(ActionData.Index))
			{
				break;
			}
			SlotLocation = OwnerSlotLocation;
			CurrentAction = NewObject<UBLAction>(this, AttackActions[ActionData.Index].LoadSynchronous());
			if (CurrentAction)
			{
				CurrentAction->OnCreateAction(this, CombatManager, Targets);
			}
			else
			{
				break;
			}
			return;
		}
		case ECombatActionType::CRYSTAL_SKILL:
		{
			if (ActionData.CrystalColor != ECrystalColor::NONE && !CrystalActions.Find(ActionData.CrystalColor)->Skills.IsValidIndex(ActionData.Index))
			{
				break;
			}

			SlotLocation = OwnerSlotLocation;

			CurrentAction = NewObject<UBLAction>(this, CrystalActions.Find(ActionData.CrystalColor)->Skills[ActionData.Index].LoadSynchronous());
			if (CurrentAction)
			{
				CurrentAction->OnCreateAction(this, CombatManager, Targets);
			}
			else
			{
				break;
			}
			return;
		}
		case ECombatActionType::SPECIAL_SKILL:
		{
			if (!SpecialActions.IsValidIndex(ActionData.Index))
			{
				break;
			}
			SlotLocation = OwnerSlotLocation;
			CurrentAction = NewObject<UBLAction>(this, SpecialActions[ActionData.Index].LoadSynchronous());
			if (CurrentAction)
			{
				CurrentAction->OnCreateAction(this, CombatManager, Targets);
			}
			else
			{
				break;
			}
			return;
		}
		default: break;
	}

	// if something went wrong, end Action
	EndAction(true);
}

void ABLCombatCharacter::DefaultAction()
{
	if (IsValid(CurrentAction))
	{
		CurrentAction->OnEndExecution.BindWeakLambda(this, [this](){ EndAction(true); });
		CurrentAction->ExecuteAction(this, nullptr);		
	}
	else
	{
		EndAction(true);
	}
}

void ABLCombatCharacter::DefaultMeleeAction()
{
	if (!IsValid(AIC) || !TargetCharacters.IsValidIndex(0) || !TargetCharacters[0])
	{
		EndAction(true);
		return;
	}

	AIC->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &ABLCombatCharacter::ReachedActionDestination);
	const FVector& Location = TargetCharacters[0]->GetActorLocation() + (TargetCharacters[0]->GetActorForwardVector() * 40);
	AIC->MoveToLocation(Location, 5.f);
}

void ABLCombatCharacter::DefaultRangeAction(TSubclassOf<ABLRangeProjectile> ProjectileClass, UPaperFlipbook* ProjectileSprite)
{
	ProjectileTargetsNum = 0;
	if (ProjectileClass && ProjectileSprite && TargetCharacters.IsValidIndex(0) && TargetCharacters[0])
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		const FVector& Location = GetActorLocation() + FVector(0.f, -30.f, 0.f);
		const FRotator& Rotation = GetActorRotation();
		ABLRangeProjectile* Projectile = GetWorld()->SpawnActor<ABLRangeProjectile>(ProjectileClass, Location, Rotation, SpawnInfo);
		if (Projectile)
		{		
			Projectile->SetData(ProjectileSprite);
			Projectile->OnReachedDestination.BindUObject(this, &ABLCombatCharacter::ReachedActionDestination);
			Projectile->FlyToTarget(TargetCharacters[0]);
			++ProjectileTargetsNum;
		}
		else
		{
			EndAction(true);
		}
	}
	else
	{
		EndAction(true);
	}
}

void ABLCombatCharacter::MultipleDefaultMeleeAction()
{
	if (!IsValid(AIC) || !TargetCharacters.IsValidIndex(0) || !TargetCharacters[0])
	{
		EndAction(true);
		return;
	}

	AIC->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &ABLCombatCharacter::ReachedActionDestination, 0);
	const FVector& Location = TargetCharacters[0]->GetActorLocation() + (TargetCharacters[0]->GetActorForwardVector() * 40);
	AIC->MoveToLocation(Location, 5.f);
}

void ABLCombatCharacter::MultipleDefaultRangeAction(TSubclassOf<ABLRangeProjectile> ProjectileClass, UPaperFlipbook* ProjectileSprite)
{
	ProjectileTargetsNum = 0;
	if (ProjectileClass && ProjectileSprite)
	{		
		ProjectileTargetIndex = 0;
		ProjectileTargetsNum = TargetCharacters.Num();
		FTimerDelegate SpawnDel;
		SpawnDel.BindUObject(this, &ABLCombatCharacter::SpawnProjectile, ProjectileClass, ProjectileSprite);
		GetWorld()->GetTimerManager().SetTimer(ProjectileSpawnTimer, SpawnDel, 0.3f, true);
	}
	else
	{
		EndAction(true);
	}
}

void ABLCombatCharacter::ColumnMeleeAction()
{
	if (!IsValid(AIC) || !TargetCharacters.IsValidIndex(0) || !TargetCharacters[0])
	{
		EndAction(true);
		return;
	}

	AIC->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &ABLCombatCharacter::ReachedActionDestination);
	AIC->MoveToActor(TargetCharacters[0], 30.f);
}

void ABLCombatCharacter::BounceRangeAction(TSubclassOf<ABLRangeProjectile> ProjectileClass, UPaperFlipbook* ProjectileSprite)
{
	ProjectileTargetIndex = 1;
	if (ProjectileClass && ProjectileSprite && TargetCharacters.IsValidIndex(0))
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		const FVector& Location = GetActorLocation() + FVector(0.f, -30.f, 0.f);
		const FRotator& Rotation = GetActorRotation();
		ABLRangeProjectile* Projectile = GetWorld()->SpawnActor<ABLRangeProjectile>(ProjectileClass, Location, Rotation, SpawnInfo);
		if (Projectile)
		{
			Projectile->SetData(ProjectileSprite);
			if (TargetCharacters.IsValidIndex(ProjectileTargetIndex))
			{
				Projectile->OnReachedDestination.BindWeakLambda(this, [this, Projectile]()
					{
						ReachedActionDestination(Projectile, 0);
					});
				Projectile->FlyToTargetBounce(TargetCharacters[0]);
			}
			else
			{
				Projectile->OnReachedDestination.BindUObject(this, &ABLCombatCharacter::ReachedActionDestination);
				Projectile->FlyToTarget(TargetCharacters[0]);
			}			
		}
		else
		{
			EndAction(true);
		}
	}
	else
	{
		EndAction(true);
	}
}

void ABLCombatCharacter::SummonAction(const TArray<ABLCombatSlot*>& Targets)
{
	if (IsValid(CurrentAction))
	{
		CurrentAction->OnEndExecution.BindWeakLambda(this, [this]() { EndAction(true); });
		CurrentAction->ExecuteAction(this, Targets);
	}
	else
	{
		EndAction(true);
	}
}

void ABLCombatCharacter::SpawnEffectsAllAction(const TArray<ABLCombatSlot*>& Targets, TSubclassOf<APaperZDCharacter> EffectClass, UPaperFlipbook* Effect)
{
	if (IsValid(CurrentAction))	
	{	
		for (auto& Slot : Targets)
		{
			if (EffectClass && Slot)
			{
				FActorSpawnParameters SpawnInfo;
				SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				const FVector Location = FVector(GetActorLocation().X, GetActorLocation().Y + 50, GetActorLocation().Z);
				const FRotator Rotation = GetActorRotation();
				APaperZDCharacter* EffectChar = GetWorld()->SpawnActor<APaperZDCharacter>(EffectClass, Slot->GetActorTransform(), SpawnInfo);
				if (EffectChar)
				{
					EffectChar->GetSprite()->SetFlipbook(Effect);
				}
			}		
		}

		CurrentAction->OnEndExecution.BindWeakLambda(this, [this]() { EndAction(true); });
		CurrentAction->ExecuteAction(this, Targets);
	}
	else
	{
		EndAction(true);
	}
}

void ABLCombatCharacter::StartActionCooldown(int32 TurnsCost)
{
	if (ClickedActionEntry)
	{
		ClickedActionEntry->bCanBeUsed = false;
		ActionsTurnsCooldown.Add(ClickedActionEntry, TurnsCost);
	}
}

float ABLCombatCharacter::CalculateElementsMultipliers(ECombatElementType DamageElementType, ECombatElementType CharacterElementType, bool& OutIsHeal)
{ 
	OutIsHeal = false;

	TArray<TArray<float>> ElementsTable = {
	  // AttackType
	  // FIRE    WATER   EARTH   WIND    ICE    THUNDER  ACID    DARK    WHITE   NONE       TargetType
		{1.0f,   2.0f,   1.0f,   1.0f,   1.5f,   1.0f,   1.0f,   1.5f,   1.0f,   1.0f},  // FIRE   
		{2.0f,   1.0f,   1.0f,   1.0f,   1.5f,   4.0f,   0.75f,  1.0f,   1.5f,   1.0f},  // WATER
		{1.0f,   1.0f,   1.0f,   2.0f,   1.0f,   0.0f,   4.0f,   1.5f,   1.0f,   1.0f},  // EARTH
		{1.0f,   1.0f,   2.0f,   1.0f,   1.0f,   4.0f,   1.0f,   1.0f,   1.5f,   1.0f},  // WIND
		{1.5f,   0.75f,  1.0f,   1.0f,   1.0f,   0.75f,  1.0f,   1.5f,   1.0f,   1.0f},  // ICE
		{1.0f,   0.0f,   4.0f,   0.0f,   1.5f,   1.0f,   1.0f,   1.0f,   1.5f,   1.0f},  // THUNDER
		{1.0f,   1.5f,   0.0f,   1.0f,   1.0f,   1.0f,   1.0f,   1.5f,   1.0f,   1.0f},  // ACID
		{0.0f,   1.0f,   0.0f,   1.0f,   0.0f,   1.0f,   0.0f,   1.0f,   2.0f,   0.0f},  // DARK
		{1.0f,   0.0f,   1.0f,   0.0f,   1.0f,   0.0f,   1.0f,   2.0f,   1.0f,   0.0f},  // WHITE
		{1.0f,   1.0f,   1.0f,   1.0f,   1.0f,   1.0f,   1.0f,   1.0f,   1.0f,   1.0f}   // NONE
	};

	const int32 AttackElementIndex = static_cast<int32>(DamageElementType);
	const int32 TargetElementIndex = static_cast<int32>(CharacterElementType);

	const float Multiplier = ElementsTable[TargetElementIndex][AttackElementIndex];

	// If Attack and Target Element is the same, damage will be converted to healing (except NONE NONE)
	if (AttackElementIndex == TargetElementIndex && AttackElementIndex != ElementsTable.Num() - 1)
	{
		OutIsHeal = true;
		return Multiplier;
	}
	else
	{
		return Multiplier;
	}
}

void ABLCombatCharacter::StartCooldown()
{
	HandleStatuses();

	GetWorld()->GetTimerManager().SetTimer(CooldownTimer, this, &ABLCombatCharacter::EndCooldown, CurrentCooldown, false);
}

void ABLCombatCharacter::EndCooldown()
{
	TargetCharacters.Empty();

	HandleTurnsCooldown();

	OnEndCooldown.ExecuteIfBound();
}

void ABLCombatCharacter::EndAction(bool bResult)
{
	if (CurrentAction)
	{
		CurrentAction->ConditionalBeginDestroy();
		CurrentAction = nullptr;
	}	
	OnActionEnded.ExecuteIfBound();
}

void ABLCombatCharacter::ReachedActionDestination(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (!IsValid(AIC) || !IsValid(CurrentAction))
	{
		UE_LOG(LogTemp, Warning, TEXT("ABLCombatCharacter::ReachedActionDestination | early return"));
		AIC->MoveToLocation(SlotLocation, 5.f);
		EndAction(true);
		return;
	}
	
	AIC->GetPathFollowingComponent()->OnRequestFinished.RemoveAll(this);
	AIC->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &ABLCombatCharacter::ReachedSlotLocation);

	CurrentAction->OnEndExecution.BindWeakLambda(this, [this](){ AIC->MoveToLocation(SlotLocation, 5.f); });
	for (ABLCombatCharacter* Target : TargetCharacters)
	{
		CurrentAction->ExecuteAction(this, Target);
	}	
}

void ABLCombatCharacter::ReachedActionDestination(FAIRequestID RequestID, const FPathFollowingResult& Result, int32 TargetIndex)
{
	if (!IsValid(AIC) || !IsValid(CurrentAction))
	{
		AIC->MoveToLocation(SlotLocation, 5.f);
		EndAction(true);
		return;
	}

	AIC->GetPathFollowingComponent()->OnRequestFinished.RemoveAll(this);

	if (TargetCharacters.IsValidIndex(TargetIndex + 1) && TargetCharacters[TargetIndex + 1] && TargetCharacters[TargetIndex + 1]->GetCurrentHP() > 0)
	{
		AIC->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &ABLCombatCharacter::ReachedActionDestination, TargetIndex + 1);
		const FVector& Location = TargetCharacters[TargetIndex + 1]->GetActorLocation() + (TargetCharacters[TargetIndex + 1]->GetActorForwardVector() * 50);
		CurrentAction->OnEndExecution.BindWeakLambda(this, [this, Location]() { AIC->MoveToLocation(Location, 5.f); });
	}
	else
	{
		AIC->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &ABLCombatCharacter::ReachedSlotLocation);
		CurrentAction->OnEndExecution.BindWeakLambda(this, [this]() { AIC->MoveToLocation(SlotLocation, 5.f); });	
	}

	CurrentAction->ExecuteAction(this, TargetCharacters[TargetIndex]);
}

void ABLCombatCharacter::ReachedActionDestination()
{
	if (!IsValid(CurrentAction) || !TargetCharacters.IsValidIndex(0) || !TargetCharacters[0])
	{
		EndAction(true);
		return;
	}

	CurrentAction->OnEndExecution.BindWeakLambda(this, [this]() { EndAction(true); });	
	CurrentAction->ExecuteAction(this, TargetCharacters[0]);
}

void ABLCombatCharacter::ReachedActionDestination(int32 Index, bool bLastProjectile)
{
	if (!IsValid(CurrentAction) || !TargetCharacters.IsValidIndex(Index) || !TargetCharacters[Index])
	{
		EndAction(true);
		return;
	}

	if (bLastProjectile)
	{
		CurrentAction->OnEndExecution.BindWeakLambda(this, [this]() { EndAction(true); });
	}

	CurrentAction->ExecuteAction(this, TargetCharacters[Index]);
}

void ABLCombatCharacter::ReachedActionDestination(ABLRangeProjectile* Projectile, int32 Index)
{
	if (!IsValid(CurrentAction) || !TargetCharacters.IsValidIndex(Index) || !TargetCharacters[Index] || !IsValid(Projectile))
	{
		EndAction(true);
		return;
	}
	
	if (!TargetCharacters[Index]->IsDead())
	{
		CurrentAction->ExecuteAction(this, TargetCharacters[Index]);
		
	}

	if (TargetCharacters.IsValidIndex(Index + 1))
	{
		Projectile->OnReachedDestination.BindWeakLambda(this, [this, Index, Projectile]()
			{
				ReachedActionDestination(Projectile, Index + 1);
			});
		Projectile->FlyToTargetBounce(TargetCharacters[Index + 1]);
	}
	else
	{
		Projectile->Destroy();
		EndAction(true);
	}
}

void ABLCombatCharacter::ReachedSlotLocation(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (IsValid(AIC))
	{		
		AIC->GetPathFollowingComponent()->OnRequestFinished.RemoveAll(this);		
	}
	EndAction(true);
}

void ABLCombatCharacter::SpawnProjectile(TSubclassOf<ABLRangeProjectile> ProjectileClass, UPaperFlipbook* ProjectileSprite)
{
	if (TargetCharacters.IsValidIndex(ProjectileTargetIndex))
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ABLRangeProjectile* Projectile = GetWorld()->SpawnActor<ABLRangeProjectile>(ProjectileClass, GetActorLocation(), GetActorRotation(), SpawnInfo);
		if (Projectile)
		{
			bool bLast = TargetCharacters.Num() - 1 == ProjectileTargetIndex ? true : false;
			Projectile->SetData(ProjectileSprite);
			Projectile->OnReachedDestination.BindUObject(this, &ABLCombatCharacter::ReachedActionDestination, ProjectileTargetIndex, bLast);
			Projectile->FlyToTarget(TargetCharacters[ProjectileTargetIndex]);
		}
		else
		{
			EndAction(true);
			return;
		}
		ProjectileTargetIndex++;
	}
	else
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(ProjectileSpawnTimer))
		{
			GetWorld()->GetTimerManager().ClearTimer(ProjectileSpawnTimer);
		}	
	}
}

void ABLCombatCharacter::GiveStatus(ECombatStatusType Status, int32 Turns)
{
	if (Statuses.Contains(Status))
	{
		Statuses[Status] = Turns;
		return;
	}

	Statuses.Add(Status, Turns);
	switch (Status)
	{
		case ECombatStatusType::STUNNED:
		{
			CurrentCooldown += BaseData.Cooldown;
			break;
		}
		case ECombatStatusType::BLINDING:
		{
			CurrentCooldown += BaseData.Cooldown * 0.5f;
			break;
		}
		case ECombatStatusType::SPEEDUP:
		{
			CurrentCooldown -= BaseData.Cooldown * 0.5f;
			break;
		}
		case ECombatStatusType::INSPIRATION:
		{
			CurrentCooldown -= BaseData.Cooldown * 0.3f;
			break;
		}
		case ECombatStatusType::SHIELD:
		{
			bDefendIdle = true;
			CurrentDefense += BaseData.BaseDefense * 3.f;
			break;
		}
		case ECombatStatusType::SNEAK:
		{
			CurrentCooldown = 0.1f;
			break;
		}
		case ECombatStatusType::INVISIBILITY:
		{
			CurrentDodge = 50.f;
			PaperFlipbook->SetSpriteColor(FLinearColor(PaperFlipbook->GetSpriteColor().R, PaperFlipbook->GetSpriteColor().G, PaperFlipbook->GetSpriteColor().B, 0.6f));
			break;
		}
		case ECombatStatusType::DMG_BOOST:
		{
			CurrentAttackDMG = BaseData.BaseAttackDMG * 1.1f;
			break;
		}
		case ECombatStatusType::ASTRAL_PROTECTION:
		{
			CurrentDefense += BaseData.BaseDefense * 3.f;
			bMagicImmunity = true;
			break;
		}
		default: break;
	}

	SetStatusDisplayVisibility(Status, true);
}

void ABLCombatCharacter::RemoveStatus(ECombatStatusType Status)
{
	if (!Statuses.Contains(Status))
	{
		return;
	}

	Statuses.Remove(Status);
	switch (Status)
	{
		case ECombatStatusType::STUNNED:
		{
			CurrentCooldown -= BaseData.Cooldown;
			break;
		}
		case ECombatStatusType::BLINDING:
		{
			CurrentCooldown -= BaseData.Cooldown * 0.5f;
			break;
		}
		case ECombatStatusType::SPEEDUP:
		{
			CurrentCooldown += BaseData.Cooldown * 0.7f;
			break;
		}
		case ECombatStatusType::INSPIRATION:
		{
			CurrentCooldown += BaseData.Cooldown * 0.3f;
			break;
		}
		case ECombatStatusType::SHIELD:
		{
			bDefendIdle = false;
			CurrentDefense -= BaseData.BaseDefense * 3.f;
			break;
		}
		case ECombatStatusType::SNEAK:
		{
			CurrentCooldown = BaseData.Cooldown;
			break;
		}
		case ECombatStatusType::INVISIBILITY:
		{
			CurrentDodge = BaseData.BaseDodge;
			PaperFlipbook->SetSpriteColor(FLinearColor(PaperFlipbook->GetSpriteColor().R, PaperFlipbook->GetSpriteColor().G, PaperFlipbook->GetSpriteColor().B, 1.f));
			break;
		}
		case ECombatStatusType::DMG_BOOST:
		{
			CurrentAttackDMG = BaseData.BaseAttackDMG;
			break;
		}
		case ECombatStatusType::ASTRAL_PROTECTION:
		{
			CurrentDefense -= BaseData.BaseDefense * 3.f;
			bMagicImmunity = false;
			break;
		}
		default: break;
	}

	SetStatusDisplayVisibility(Status, false);
}

void ABLCombatCharacter::RemoveStatuses(TArray<ECombatStatusType> StatusesToDelete)
{
	for (const auto& Status : StatusesToDelete)
	{
		RemoveStatus(Status);
	}
}

void ABLCombatCharacter::HandleStatuses()
{
	TArray<ECombatStatusType> StatusesToDelete;

	float SimpleDMG = 0.f;

	for (auto& Status : Statuses)
	{
		switch (Status.Key)
		{
			case ECombatStatusType::BLEEDING:
			case ECombatStatusType::POISONING:
			{
				SimpleDMG += BaseData.MaxHP * 0.05f;
				break;
			}
			case ECombatStatusType::FLAMING:
			{
				SimpleDMG += BaseData.MaxHP * 0.1f;
				break;
			}
			default: break;
		}

		Status.Value--;	

		if (Status.Value == 0)
		{
			StatusesToDelete.Add(Status.Key);
		}
	}

	TakeSimpleDamage(SimpleDMG);

	FTimerHandle RemoveDelay;
	FTimerDelegate RemoveDel;
	RemoveDel.BindUObject(this, &ABLCombatCharacter::RemoveStatuses, StatusesToDelete);
	GetWorld()->GetTimerManager().SetTimer(RemoveDelay, RemoveDel, 1.f, false);
}

void ABLCombatCharacter::TakeSimpleDamage(float Damage)
{
	if (Damage <= 0.f)
	{
		return;
	}

	Damage = FMath::RoundHalfFromZero(Damage);
	const float NewHP = CurrentHP - Damage;
	CurrentHP = FMath::Clamp(NewHP, 0, BaseData.MaxHP);
	DisplayTextDMG(Damage, false, ECombatElementType::NONE);

	if (BaseData.TakeDMGAnim && GetAnimInstance())
	{
		GetAnimationComponent()->GetAnimInstance()->PlayAnimationOverride(BaseData.TakeDMGAnim);
	}

	OnHealthUpdate.ExecuteIfBound();
	if (CurrentHP <= 0.f)
	{
		bDead = true;
		StatusDisplay->SetVisibility(false);
		OnDeath.ExecuteIfBound();
	}
}

void ABLCombatCharacter::HandleHealHit(float Damage, float HealMultiplier, ECombatElementType HealElementType)
{
	const float HealValue = Damage * HealMultiplier;
	CurrentHP = FMath::Clamp((CurrentHP + HealValue), 0, BaseData.MaxHP);
	DisplayTextDMG(HealValue, true, HealElementType);
	OnHealthUpdate.ExecuteIfBound();
}

void ABLCombatCharacter::HandleDamageHit(ABLCombatCharacter* Attacker, float Damage, float DMGMultiplier, ECombatElementType DamageElementType, bool bMagicalAction)
{
	// Only if action is physical
	if (!bMagicalAction)
	{
		// If it draws DODGE, character will not take any damage or heal
		const int32 DodgeChance = FMath::RandRange(1, 100);
		if (DodgeChance <= CurrentDodge)
		{
			DisplayTextDMG(0, false, DamageElementType, true);
			return;
		}
	}
	
	if (bMagicalAction && bMagicImmunity)
	{
		DisplayTextDMG(0, false, DamageElementType, false);
		return;
	}

	// If it draws Pierce, Defense is reduced by half
	const int32 PierceChance = FMath::RandRange(1, 100);
	const float NewDefense = PierceChance <= BaseData.Pierce ? CurrentDefense / 2 : CurrentDefense;

	// 10 def decreases dmg by 5%
	float DMGValue = (Damage * DMGMultiplier) * (1.f - ((NewDefense / 1000) * 5));

	// If attack is physical and Attacker has Poisoning status, dmg is decreased by 20%
	if (!bMagicalAction && Attacker->Statuses.Contains(ECombatStatusType::POISONING))
	{
		// Clamp because Defense can be higher than Damage, so that Damage is not negative
		DMGValue = FMath::Clamp(FMath::RoundHalfFromZero(DMGValue * 0.8f), 0, FMath::RoundHalfFromZero(DMGValue));
	}
	else
	{
		DMGValue = FMath::Clamp(FMath::RoundHalfFromZero(DMGValue), 0, FMath::RoundHalfFromZero(DMGValue));
	}

	// Sets character HP between 0 and MaxHP
	CurrentHP = FMath::Clamp((CurrentHP - DMGValue), 0, BaseData.MaxHP);

	DisplayTextDMG(DMGValue, false, DamageElementType);
	OnHealthUpdate.ExecuteIfBound();

	if (BaseData.TakeDMGAnim && GetAnimInstance() && !bDefendIdle)
	{
		GetAnimationComponent()->GetAnimInstance()->PlayAnimationOverride(BaseData.TakeDMGAnim);
	}
}

void ABLCombatCharacter::HandleHitStatuses(ABLCombatCharacter* Attacker, const TArray<FCombatStatus>& InStatuses, bool bMagicalAction)
{
	// Temp array to add weapon and action statuses (weapon status only if its physical action)
	TArray<FCombatStatus> AllStatuses = InStatuses;
	if (!bMagicalAction)
	{
		AllStatuses.Add(Attacker->GetWeaponStatus());
	}

	for (const auto& Status : AllStatuses)
	{
		if (BaseData.StatusesImmunity.Contains(Status.Status))
		{
			continue;
		}
		const int32 ApplicationChance = FMath::RandRange(1, 100);
		if (ApplicationChance <= Status.ApplicationChance)
		{
			GiveStatus(Status.Status, Status.Turns);
		}
	}
}

void ABLCombatCharacter::HandleHitByAction(ABLCombatCharacter* Attacker, float Damage, ECombatElementType DamageElementType, bool bMagical, const TArray<FCombatStatus>& InStatuses)
{
	if (!Attacker)
	{
		return;
	}

	bool bIsHeal = false;

	const ECombatElementType DamageElement = bMagical ? DamageElementType : Attacker->GetWeaponElement();
	const float DMGMultiplier = CalculateElementsMultipliers(DamageElement, BaseData.Element, bIsHeal);

	if (bIsHeal)
	{
		HandleHealHit(Damage, DMGMultiplier, DamageElement);
	}
	else
	{	
		HandleDamageHit(Attacker, Damage, DMGMultiplier, DamageElement, bMagical);
	}

	HandleHitStatuses(Attacker, InStatuses, bMagical);

	if (CurrentHP <= 0.f)
	{
		bDead = true;
		StatusDisplay->SetVisibility(false);
		OnDeath.ExecuteIfBound();
	}
}



