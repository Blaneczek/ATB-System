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

ABLCombatCharacter::ABLCombatCharacter()
{
	DMGDisplay = CreateDefaultSubobject<UWidgetComponent>(TEXT("DMG Display"));
	DMGDisplay->SetupAttachment(PaperFlipbook);

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	AIC = nullptr;
	CurrentAction = nullptr;
	Movement->MaxWalkSpeed = 900.f;
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

void ABLCombatCharacter::SetData(const FCombatCharData& InBaseData, const TArray<TSoftClassPtr<UBLAction>>& InAttackActions, const TArray<TSoftClassPtr<UBLAction>>& InDefendActions, const TMap<ECrystalColor, FCrystalSkills>& InCrystalActions, const TArray<TSoftClassPtr<UBLAction>>& InSpecialActions, const FTransform& InSlotTransform)
{
	SetData(InBaseData, InAttackActions, InDefendActions);

	CrystalActions = InCrystalActions;
	SpecialActions = InSpecialActions;
	SlotTransform = InSlotTransform;
}

void ABLCombatCharacter::SetData(const FCombatCharData& InBaseData, const TArray<TSoftClassPtr<UBLAction>>& InAttackActions, const TArray<TSoftClassPtr<UBLAction>>& InDefendActions)
{
	bDefendIdle = false;
	BaseData = InBaseData;
	CurrentHP = BaseData.MaxHP;
	CurrentME = BaseData.MaxME;
	CurrentDefense = BaseData.BaseDefense;

	AttackActions = InAttackActions;
	DefendActions = InDefendActions;	

	PaperFlipbook->SetFlipbook(BaseData.Sprite);
	GetAnimationComponent()->SetAnimInstanceClass(BaseData.AnimInstanceClass);
}


void ABLCombatCharacter::CreateAction(const FVector& OwnerSlotLocation, ECombatActionType ActionType, int32 ActionIndex, const TArray<ABLCombatCharacter*>& Targets, ECrystalColor CrystalColor, UObject* InClickedActionEntry)
{
	ClickedActionEntry = Cast<UBLActionEntryData>(InClickedActionEntry);

	switch (ActionType)
	{
		case ECombatActionType::ATTACK:
		{
			if (!AttackActions.IsValidIndex(ActionIndex))
			{							
				EndAction(true);
				return;
			}

			SlotLocation = OwnerSlotLocation;
			TargetCharacters = Targets;

			CurrentAction = NewObject<UBLAction>(this, AttackActions[ActionIndex].LoadSynchronous());
			if (CurrentAction)
			{
				CurrentAction->OnCreateAction(this);
			}
			return;
		}
		case ECombatActionType::DEFEND:
		{
			if (!DefendActions.IsValidIndex(ActionIndex))
			{
				EndAction(true);
				return;
			}

			CurrentAction = NewObject<UBLAction>(this, DefendActions[ActionIndex].LoadSynchronous());
			if (CurrentAction)
			{
				CurrentAction->OnCreateAction(this);
			}
			return;
		}
		case ECombatActionType::CRYSTAL_SKILL:
		{
			if (CrystalColor != ECrystalColor::NONE && !CrystalActions.Find(CrystalColor)->Skills.IsValidIndex(ActionIndex))
			{
				EndAction(true);
				return;
			}

			SlotLocation = OwnerSlotLocation;
			TargetCharacters = Targets;

			CurrentAction = NewObject<UBLAction>(this, CrystalActions.Find(CrystalColor)->Skills[ActionIndex].LoadSynchronous());
			if (CurrentAction)
			{
				CurrentAction->OnCreateAction(this);
			}
			return;
		}
		case ECombatActionType::SPECIAL_SKILL:
		{
			if (!SpecialActions.IsValidIndex(ActionIndex))
			{
				EndAction(true);
				return;
			}

			SlotLocation = OwnerSlotLocation;
			TargetCharacters = Targets;

			CurrentAction = NewObject<UBLAction>(this, SpecialActions[ActionIndex].LoadSynchronous());
			if (CurrentAction)
			{
				CurrentAction->OnCreateAction(this);
			}
			return;
		}
		case ECombatActionType::ITEM:
		{
			//TODO
			return;
		}
		case ECombatActionType::RUN_AWAY:
		{
			//TODO
			return;
		}
	}
}

void ABLCombatCharacter::DefaultAction()
{
	if (CurrentAction)
	{
		CurrentAction->OnEndExecution.BindLambda([this](){ EndAction(true); });
		CurrentAction->ExecuteAction(this, nullptr);		
	}
}

void ABLCombatCharacter::DefaultMeleeAction()
{
	if (!IsValid(AIC) || !TargetCharacters.IsValidIndex(0) || !TargetCharacters[0])
	{
		return;
	}

	AIC->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &ABLCombatCharacter::ReachedActionDestination);
	UE_LOG(LogTemp, Warning, TEXT("STARTED"));
	AIC->MoveToActor(TargetCharacters[0], 30.f);
}

void ABLCombatCharacter::DefaultRangeAction(TSubclassOf<ABLRangeProjectile> ProjectileClass, UPaperFlipbook* ProjectileSprite)
{
	if (ProjectileClass)
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ABLRangeProjectile* Projectile = GetWorld()->SpawnActor<ABLRangeProjectile>(ProjectileClass, GetActorLocation(), GetActorRotation(), SpawnInfo);
		if (Projectile)
		{
			Projectile->SetData(ProjectileSprite);
			Projectile->OnReachedDestination.BindUObject(this, &ABLCombatCharacter::ReachedActionDestination);
			Projectile->FlyToTarget(TargetCharacters[0]);
		}
	}
}

void ABLCombatCharacter::MultipleDefaultMeleeAction()
{
	if (!IsValid(AIC) || !TargetCharacters.IsValidIndex(0) || !TargetCharacters[0])
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("STARTED"));
	AIC->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &ABLCombatCharacter::ReachedActionDestination, 0);
	AIC->MoveToActor(TargetCharacters[0], 30.f);
}

void ABLCombatCharacter::StartActionCooldown(int32 TurnsCost)
{
	if (ClickedActionEntry)
	{
		ClickedActionEntry->bCanBeUsed = false;
		ActionsTurnsCooldown.Add(ClickedActionEntry, TurnsCost);
	}
}

/* Do it maybe 
void ABLCombatCharacter::StepForward()
{
	if (AIC)
	{
		AIC->MoveToLocation(GetActorLocation() + FVector(-50.f, 0.f, 0.f), 5.f);
	}
}

void ABLCombatCharacter::BackInLine()
{
	if (AIC)
	{
		AIC->GetPathFollowingComponent()->OnRequestFinished.AddLambda([this](FAIRequestID RequestID, const FPathFollowingResult& Result) {SetActorTransform(SlotTransform); });
		CurrentAction->OnEndExecution.BindLambda([this]() { AIC->MoveToLocation(SlotLocation, 5.f); });
		AIC->GetPathFollowingComponent()->OnRequestFinished.RemoveAll(this);
	}	
}
*/

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
		{1.0f,   1.0f,   1.0f,   1.0f,   1.0f,   0.0f,   1.0f,   1.0f,   1.0f,   1.0f}   // NONE
	};

	const uint8 AttackIndex = static_cast<int>(DamageElementType);
	const uint8 TargetIndex = static_cast<int>(CharacterElementType);

	const float Multiplier = ElementsTable[TargetIndex][AttackIndex];

	// If Attack and Target Element is the same, heals target
	if (AttackIndex == TargetIndex)
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
	GetWorld()->GetTimerManager().SetTimer(CooldownTimer, this, &ABLCombatCharacter::EndCooldown, BaseData.Cooldown, false);
}

void ABLCombatCharacter::EndCooldown()
{
	CurrentDefense = BaseData.BaseDefense;
	bDefendIdle = false;

	TArray<UBLActionEntryData*> EntriesToDelete;

	for (auto& Item : ActionsTurnsCooldown)
	{	
		if (Item.Key->bCanBeUsed) continue;

		--Item.Value;
		FFormatNamedArguments Args;
		Args.Add(TEXT("Name"), Item.Key->TempName);
		Args.Add(TEXT("Turns"), Item.Value);
		const FText NewName = FText::Format(FText::FromString("{Name} Cd:{Turns}t"), Args);
		Item.Key->ChangeName(NewName);
		if (Item.Value <= 0)
		{
			Item.Key->ChangeName(Item.Key->TempName);
			Item.Key->bCanBeUsed = true;
			EntriesToDelete.Add(Item.Key);
		}
	}

	for (auto* Item : EntriesToDelete)
	{
		if (ActionsTurnsCooldown.Contains(Item))
		{
			ActionsTurnsCooldown.Remove(Item);
		}
	}
	OnEndCooldown.ExecuteIfBound();
}

void ABLCombatCharacter::EndAction(bool bResult)
{
	UE_LOG(LogTemp, Warning, TEXT("End Action"));
	CurrentAction->ConditionalBeginDestroy();
	CurrentAction = nullptr;
	OnActionEnded.ExecuteIfBound();
	StartCooldown();
}

void ABLCombatCharacter::ReachedActionDestination(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (!IsValid(AIC) || !IsValid(CurrentAction) || !TargetCharacters.IsValidIndex(0) || !TargetCharacters[0])
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("REACHED"));
	
	AIC->GetPathFollowingComponent()->OnRequestFinished.RemoveAll(this);
	AIC->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &ABLCombatCharacter::ReachedSlotLocation);

	CurrentAction->OnEndExecution.BindLambda([this](){ AIC->MoveToLocation(SlotLocation, 5.f); });
	CurrentAction->ExecuteAction(this, TargetCharacters[0]);
}

void ABLCombatCharacter::ReachedActionDestination(FAIRequestID RequestID, const FPathFollowingResult& Result, int32 TargetIndex)
{
	if (!IsValid(AIC) || !IsValid(CurrentAction))
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("REACHED"));
	AIC->GetPathFollowingComponent()->OnRequestFinished.RemoveAll(this);

	if (TargetCharacters.IsValidIndex(TargetIndex + 1) && TargetCharacters[TargetIndex + 1] && TargetCharacters[TargetIndex + 1]->GetCurrentHP() > 0)
	{
		AIC->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &ABLCombatCharacter::ReachedActionDestination, TargetIndex + 1);
		CurrentAction->OnEndExecution.BindLambda([this, TargetIndex]() { AIC->MoveToActor(TargetCharacters[TargetIndex + 1], 10.f); });
	}
	else
	{
		AIC->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &ABLCombatCharacter::ReachedSlotLocation);
		CurrentAction->OnEndExecution.BindLambda([this]() { AIC->MoveToLocation(SlotLocation, 5.f); });	
	}

	CurrentAction->ExecuteAction(this, TargetCharacters[TargetIndex]);
}

// for projectile
void ABLCombatCharacter::ReachedActionDestination()
{
	if (!IsValid(CurrentAction) || !TargetCharacters.IsValidIndex(0) || !TargetCharacters[0])
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("REACHED projectile"));	
	CurrentAction->OnEndExecution.BindLambda([this]() { EndAction(true); });
	CurrentAction->ExecuteAction(this, TargetCharacters[0]);
}

void ABLCombatCharacter::ReachedSlotLocation(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (IsValid(AIC))
	{		
		AIC->GetPathFollowingComponent()->OnRequestFinished.RemoveAll(this);
		EndAction(true);
	}
}


void ABLCombatCharacter::HandleHitByAction(float Damage, ECombatElementType DamageElementType)
{
	bool bIsHeal = false;
	const float DMGMultiplier = CalculateElementsMultipliers(DamageElementType, BaseData.Element, bIsHeal);

	if (bIsHeal)
	{
		const float HealValue = Damage * DMGMultiplier;
		CurrentHP = FMath::Clamp((CurrentHP + HealValue), 0, BaseData.MaxHP);
		DisplayTextDMG(HealValue, true, DamageElementType);
		if (BaseData.HealAnim)
		{
			GetAnimationComponent()->GetAnimInstance()->PlayAnimationOverride(BaseData.HealAnim);
		}
	}
	else
	{
		//TODO: change how Defense works	
		const float DMGValue = DMGMultiplier > 0 ? (Damage * DMGMultiplier) - CurrentDefense : 0.f;
		CurrentHP = FMath::Clamp((CurrentHP - DMGValue), 0, BaseData.MaxHP);
		DisplayTextDMG(DMGValue, false, DamageElementType);
		if (BaseData.TakeDMGAnim)
		{
			GetAnimationComponent()->GetAnimInstance()->PlayAnimationOverride(BaseData.TakeDMGAnim);
		}
	}

	OnHealthUpdate.ExecuteIfBound();

	if (CurrentHP <= 0.f)
	{
		//TODO: add death animation
		//TODO: change idle anim to death
		OnDeath.ExecuteIfBound();
	}
}



