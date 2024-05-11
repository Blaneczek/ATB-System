// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLActionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BLRangeProjectile.h"
#include "Navigation/PathFollowingComponent.h"
#include "Actions/BLAction.h"
#include "Components/WidgetComponent.h"
#include "UI/Entries/BLActionEntryData.h"
#include "UI/Entries/BLItemEntryData.h"
#include "BladeOfLegend/DAWID/Items/BLCombatItem.h"
#include "Kismet/KismetMathLibrary.h"
#include "BLCombatSlot.h"
#include "BLCombatCharacter.h"

// Sets default values for this component's properties
UBLActionComponent::UBLActionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	AIC = nullptr;
	CurrentAction = nullptr;
	ProjectileTargetsNum = 0;
	ProjectileTargetIndex = 0;
}


// Called when the game starts
void UBLActionComponent::BeginPlay()
{
	Super::BeginPlay();	

	ABLCombatCharacter* OwnerChar = Cast<ABLCombatCharacter>(GetOwner());
	if (OwnerChar)
	{
		AIC = Cast<AAIController>(OwnerChar->GetController());
	}
}

void UBLActionComponent::CreateAction(const FVector& OwnerSlotLocation, const TArray<ABLCombatSlot*>& Targets, const FCombatActionData& ActionData, AActor* CombatManagerActor)
{
	ABLCombatCharacter* OwnerChar = Cast<ABLCombatCharacter>(GetOwner());
	if (!OwnerChar)
	{
		EndAction(true);
		return;
	}

	SlotLocation = OwnerSlotLocation;
	TargetSlots = Targets;

	switch (ActionData.Type)
	{
		case ECombatActionType::ATTACK:
		{
			if (!AttackActions.IsValidIndex(ActionData.Index))
			{
				break;
			}

			CurrentAction = NewObject<UBLAction>(this, AttackActions[ActionData.Index].LoadSynchronous());
			if (CurrentAction)
			{
				CurrentAction->OnCreateAction(this, OwnerChar, CombatManagerActor);
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
				CurrentAction->OnCreateAction(this, OwnerChar, CombatManagerActor);
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

			CurrentAction = NewObject<UBLAction>(this, CrystalActions.Find(ActionData.CrystalColor)->Skills[ActionData.Index].LoadSynchronous());
			if (CurrentAction)
			{
				CurrentAction->OnCreateAction(this, OwnerChar, CombatManagerActor);
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

			CurrentAction = NewObject<UBLAction>(this, SpecialActions[ActionData.Index].LoadSynchronous());
			if (CurrentAction)
			{
				CurrentAction->OnCreateAction(this, OwnerChar, CombatManagerActor);
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
				CurrentAction->OnCreateAction(this, OwnerChar, CombatManagerActor);
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
				OnEscapeAction.ExecuteIfBound(true);
			}
			else
			{
				OnEscapeAction.ExecuteIfBound(false);
				EndAction(true);
			}
			return;
		}
	}

	// if something went wrong, end Action
	EndAction(true);
}

void UBLActionComponent::DefaultAction()
{
	if (IsValid(CurrentAction))
	{
		CurrentAction->OnEndExecution.BindWeakLambda(this, [this]() { EndAction(true); });
		CurrentAction->ExecuteAction(nullptr);
	}
	else
	{
		EndAction(true);
	}
}

void UBLActionComponent::DefaultMeleeAction()
{
	if (!IsValid(AIC) || !TargetSlots.IsValidIndex(0) || !TargetSlots[0])
	{
		EndAction(true);
		return;
	}

	AIC->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &UBLActionComponent::ReachedActionDestination);
	const FVector& Location = TargetSlots[0]->GetCharacter()->GetActorLocation() + (TargetSlots[0]->GetCharacter()->GetActorForwardVector() * 40);
	AIC->MoveToLocation(Location, 5.f);
}

void UBLActionComponent::DefaultRangeAction(TSubclassOf<ABLRangeProjectile> ProjectileClass, UPaperFlipbook* ProjectileSprite)
{
	ProjectileTargetsNum = 0;
	if (ProjectileClass && ProjectileSprite && TargetSlots.IsValidIndex(0) && TargetSlots[0])
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		const FVector& Location = GetOwner()->GetActorLocation() + FVector(0.f, -30.f, 0.f);
		const FRotator& Rotation = GetOwner()->GetActorRotation();
		ABLRangeProjectile* Projectile = GetWorld()->SpawnActor<ABLRangeProjectile>(ProjectileClass, Location, Rotation, SpawnInfo);
		if (Projectile)
		{
			Projectile->SetData(ProjectileSprite);
			Projectile->OnReachedDestination.BindUObject(this, &UBLActionComponent::ReachedActionDestination);
			Projectile->FlyToTarget(TargetSlots[0]->GetCharacter());
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

void UBLActionComponent::MultipleDefaultMeleeAction()
{
	if (!IsValid(AIC) || !TargetSlots.IsValidIndex(0) || !TargetSlots[0])
	{
		EndAction(true);
		return;
	}

	AIC->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &UBLActionComponent::ReachedActionDestination, 0);
	const FVector& Location = TargetSlots[0]->GetCharacter()->GetActorLocation() + (TargetSlots[0]->GetCharacter()->GetActorForwardVector() * 40);
	AIC->MoveToLocation(Location, 5.f);
}

void UBLActionComponent::MultipleDefaultRangeAction(TSubclassOf<ABLRangeProjectile> ProjectileClass, UPaperFlipbook* ProjectileSprite)
{
	ProjectileTargetsNum = 0;
	if (ProjectileClass && ProjectileSprite)
	{
		ProjectileTargetIndex = 0;
		ProjectileTargetsNum = TargetSlots.Num();
		FTimerDelegate SpawnDel;
		SpawnDel.BindUObject(this, &UBLActionComponent::SpawnProjectile, ProjectileClass, ProjectileSprite);
		GetWorld()->GetTimerManager().SetTimer(ProjectileSpawnTimer, SpawnDel, 0.3f, true);
	}
	else
	{
		EndAction(true);
	}
}

void UBLActionComponent::ColumnMeleeAction()
{
	if (!IsValid(AIC) || !TargetSlots.IsValidIndex(0) || !TargetSlots[0])
	{
		EndAction(true);
		return;
	}

	AIC->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &UBLActionComponent::ReachedActionDestination);
	const FVector& Location = TargetSlots[0]->GetCharacter()->GetActorLocation() + (TargetSlots[0]->GetCharacter()->GetActorForwardVector() * 40);
	AIC->MoveToLocation(Location, 5.f);
}

void UBLActionComponent::BounceRangeAction(TSubclassOf<ABLRangeProjectile> ProjectileClass, UPaperFlipbook* ProjectileSprite)
{
	ProjectileTargetIndex = 1;
	if (ProjectileClass && ProjectileSprite && TargetSlots.IsValidIndex(0))
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		const FVector& Location = GetOwner()->GetActorLocation() + FVector(0.f, -30.f, 0.f);
		const FRotator& Rotation = GetOwner()->GetActorRotation();
		ABLRangeProjectile* Projectile = GetWorld()->SpawnActor<ABLRangeProjectile>(ProjectileClass, Location, Rotation, SpawnInfo);
		if (Projectile)
		{
			Projectile->SetData(ProjectileSprite);
			if (TargetSlots.IsValidIndex(ProjectileTargetIndex))
			{
				Projectile->OnReachedDestination.BindWeakLambda(this, [this, Projectile]()
					{
						ReachedActionDestination(Projectile, 0);
					});
				Projectile->FlyToTargetBounce(TargetSlots[0]->GetCharacter());
			}
			else
			{
				Projectile->OnReachedDestination.BindUObject(this, &UBLActionComponent::ReachedActionDestination);
				Projectile->FlyToTarget(TargetSlots[0]->GetCharacter());
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

void UBLActionComponent::MultipleInPlaceAction(TSubclassOf<APaperZDCharacter> EffectClass)
{
	if (IsValid(CurrentAction))
	{
		for (auto& Slot : TargetSlots)
		{
			if (EffectClass && Slot)
			{
				FActorSpawnParameters SpawnInfo;
				SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				APaperZDCharacter* EffectChar = GetWorld()->SpawnActor<APaperZDCharacter>(EffectClass, Slot->GetActorTransform(), SpawnInfo);
			}
		}

		CurrentAction->OnEndExecution.BindWeakLambda(this, [this]() { EndAction(true); });
		CurrentAction->ExecuteAction(TargetSlots);
	}
	else
	{
		EndAction(true);
	}
}

// For DefaultMelee and ColumnMelee action flow.
void UBLActionComponent::ReachedActionDestination(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (!IsValid(AIC) || !IsValid(CurrentAction))
	{
		AIC->MoveToLocation(SlotLocation, 5.f);
		EndAction(true);
		return;
	}

	AIC->GetPathFollowingComponent()->OnRequestFinished.RemoveAll(this);
	AIC->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &UBLActionComponent::ReachedSlotLocation);

	CurrentAction->OnEndExecution.BindWeakLambda(this, [this]() { AIC->MoveToLocation(SlotLocation, 5.f); });
	CurrentAction->ExecuteAction(TargetSlots);
}

// For MultipleMelee action flow.
void UBLActionComponent::ReachedActionDestination(FAIRequestID RequestID, const FPathFollowingResult& Result, int32 TargetIndex)
{
	if (!IsValid(AIC) || !IsValid(CurrentAction))
	{
		AIC->MoveToLocation(SlotLocation, 5.f);
		EndAction(true);
		return;
	}

	AIC->GetPathFollowingComponent()->OnRequestFinished.RemoveAll(this);

	// If there is another target call this function again after reaching location.
	if (TargetSlots.IsValidIndex(TargetIndex + 1) && TargetSlots[TargetIndex + 1] && !TargetSlots[TargetIndex + 1]->GetCharacter()->IsDead())
	{
		AIC->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &UBLActionComponent::ReachedActionDestination, TargetIndex + 1);
		const FVector& Location = TargetSlots[TargetIndex + 1]->GetCharacter()->GetActorLocation() + (TargetSlots[TargetIndex + 1]->GetCharacter()->GetActorForwardVector() * 50);
		CurrentAction->OnEndExecution.BindWeakLambda(this, [this, Location]() { AIC->MoveToLocation(Location, 5.f); });
	}
	// If it was last target, return do slot.
	else
	{
		AIC->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &UBLActionComponent::ReachedSlotLocation);
		CurrentAction->OnEndExecution.BindWeakLambda(this, [this]() { AIC->MoveToLocation(SlotLocation, 5.f); });
	}

	CurrentAction->ExecuteAction(TargetSlots[TargetIndex]);
}

// For DeafultRange action flow.
void UBLActionComponent::ReachedActionDestination()
{
	if (!IsValid(CurrentAction) || !TargetSlots.IsValidIndex(0) || !TargetSlots[0])
	{
		EndAction(true);
		return;
	}

	CurrentAction->OnEndExecution.BindWeakLambda(this, [this]() { EndAction(true); });
	CurrentAction->ExecuteAction(TargetSlots[0]);
}


void UBLActionComponent::ReachedActionDestination(int32 Index, bool bLastProjectile)
{
	if (!IsValid(CurrentAction) || !TargetSlots.IsValidIndex(Index) || !TargetSlots[Index])
	{
		EndAction(true);
		return;
	}

	if (bLastProjectile)
	{
		CurrentAction->OnEndExecution.BindWeakLambda(this, [this]() { EndAction(true); });
	}

	CurrentAction->ExecuteAction(TargetSlots[Index]);
}

void UBLActionComponent::ReachedActionDestination(ABLRangeProjectile* Projectile, int32 Index)
{
	if (!IsValid(CurrentAction) || !TargetSlots.IsValidIndex(Index) || !TargetSlots[Index] || !IsValid(Projectile))
	{
		EndAction(true);
		return;
	}

	if (!TargetSlots[Index]->GetCharacter()->IsDead())
	{
		CurrentAction->ExecuteAction(TargetSlots[Index]);

	}

	if (TargetSlots.IsValidIndex(Index + 1))
	{
		Projectile->OnReachedDestination.BindWeakLambda(this, [this, Index, Projectile]()
			{
				ReachedActionDestination(Projectile, Index + 1);
			});
		Projectile->FlyToTargetBounce(TargetSlots[Index + 1]->GetCharacter());
	}
	else
	{
		Projectile->Destroy();
		EndAction(true);
	}
}

void UBLActionComponent::ReachedSlotLocation(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (IsValid(AIC))
	{
		AIC->GetPathFollowingComponent()->OnRequestFinished.RemoveAll(this);
	}
	EndAction(true);
}

void UBLActionComponent::SpawnProjectile(TSubclassOf<ABLRangeProjectile> ProjectileClass, UPaperFlipbook* ProjectileSprite)
{
	if (TargetSlots.IsValidIndex(ProjectileTargetIndex))
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ABLRangeProjectile* Projectile = GetWorld()->SpawnActor<ABLRangeProjectile>(ProjectileClass, GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation(), SpawnInfo);
		if (Projectile)
		{
			bool bLast = TargetSlots.Num() - 1 == ProjectileTargetIndex ? true : false;
			Projectile->SetData(ProjectileSprite);
			Projectile->OnReachedDestination.BindUObject(this, &UBLActionComponent::ReachedActionDestination, ProjectileTargetIndex, bLast);
			Projectile->FlyToTarget(TargetSlots[ProjectileTargetIndex]->GetCharacter());
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

void UBLActionComponent::EndAction(bool bResult)
{
	TargetSlots.Empty();

	if (CurrentAction)
	{
		CurrentAction->ConditionalBeginDestroy();
		CurrentAction = nullptr;
	}
	OnActionFinished.ExecuteIfBound();
}

void UBLActionComponent::SetActions(const FCombatActions& InCombatActions)
{
	AttackActions = InCombatActions.AttackActions;
	DefendAction = InCombatActions.DefendAction;
	SpecialActions = InCombatActions.SpecialActions;
	CrystalActions = InCombatActions.CrystalActions;
	ItemActions = InCombatActions.ItemActions;
	RunAwayAction = InCombatActions.RunAwayAction;
}

void UBLActionComponent::SetActions(const TArray<TSoftClassPtr<UBLAction>>& InActions)
{
	AttackActions = InActions;
}

