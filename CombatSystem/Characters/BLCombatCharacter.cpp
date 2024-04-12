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

	BleedingDisplay = CreateDefaultSubobject<UWidgetComponent>(TEXT("Bleeding Display"));
	BleedingDisplay->SetupAttachment(PaperFlipbook);
	BleedingDisplay->SetVisibility(false);

	PoisoningDisplay = CreateDefaultSubobject<UWidgetComponent>(TEXT("Poisoning Display"));
	PoisoningDisplay->SetupAttachment(PaperFlipbook);
	PoisoningDisplay->SetVisibility(false);

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

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
	SetData(InBaseData, InCombatActions.AttackActions, InCombatActions.DefendActions);

	CrystalActions = InCombatActions.CrystalActions;
	SpecialActions = InCombatActions.SpecialActions;
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


void ABLCombatCharacter::CreateAction(const FVector& OwnerSlotLocation, const TArray<ABLCombatCharacter*>& Targets, const FCombatActionData& ActionData)
{
	ClickedActionEntry = Cast<UBLActionEntryData>(ActionData.ActionEntry);

	switch (ActionData.Type)
	{
		case ECombatActionType::ATTACK:
		{
			if (!AttackActions.IsValidIndex(ActionData.Index))
			{							
				EndAction(true);
				return;
			}
			SlotLocation = OwnerSlotLocation;
			TargetCharacters = Targets;

			CurrentAction = NewObject<UBLAction>(this, AttackActions[ActionData.Index].LoadSynchronous());
			if (CurrentAction)
			{
				CurrentAction->OnCreateAction(this);
			}
			return;
		}
		case ECombatActionType::DEFEND:
		{
			if (!DefendActions.IsValidIndex(ActionData.Index))
			{
				EndAction(true);
				return;
			}

			CurrentAction = NewObject<UBLAction>(this, DefendActions[ActionData.Index].LoadSynchronous());
			if (CurrentAction)
			{
				CurrentAction->OnCreateAction(this);
			}
			return;
		}
		case ECombatActionType::CRYSTAL_SKILL:
		{
			if (ActionData.CrystalColor != ECrystalColor::NONE && !CrystalActions.Find(ActionData.CrystalColor)->Skills.IsValidIndex(ActionData.Index))
			{
				EndAction(true);
				return;
			}

			SlotLocation = OwnerSlotLocation;
			TargetCharacters = Targets;

			CurrentAction = NewObject<UBLAction>(this, CrystalActions.Find(ActionData.CrystalColor)->Skills[ActionData.Index].LoadSynchronous());
			if (CurrentAction)
			{
				CurrentAction->OnCreateAction(this);
			}
			return;
		}
		case ECombatActionType::SPECIAL_SKILL:
		{
			if (!SpecialActions.IsValidIndex(ActionData.Index))
			{
				EndAction(true);
				return;
			}

			SlotLocation = OwnerSlotLocation;
			TargetCharacters = Targets;

			CurrentAction = NewObject<UBLAction>(this, SpecialActions[ActionData.Index].LoadSynchronous());
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
	if (IsValid(CurrentAction))
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
	ProjectileTargetsNum = 0;
	if (ProjectileClass && ProjectileSprite)
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ABLRangeProjectile* Projectile = GetWorld()->SpawnActor<ABLRangeProjectile>(ProjectileClass, GetActorLocation(), GetActorRotation(), SpawnInfo);
		if (Projectile)
		{		
			Projectile->SetData(ProjectileSprite);
			Projectile->OnReachedDestination.BindUObject(this, &ABLCombatCharacter::ReachedActionDestination);
			Projectile->FlyToTarget(TargetCharacters[0]);
			++ProjectileTargetsNum;
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
}

void ABLCombatCharacter::ColumnMeleeAction()
{
	if (!IsValid(AIC) || !TargetCharacters.IsValidIndex(0) || !TargetCharacters[0])
	{
		return;
	}

	AIC->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &ABLCombatCharacter::ReachedActionDestination);
	UE_LOG(LogTemp, Warning, TEXT("STARTED"));
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

//Do it maybe 
/* 
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

	const uint8 AttackElementIndex = static_cast<int32>(DamageElementType);
	const uint8 TargetElementIndex = static_cast<int32>(CharacterElementType);

	const float Multiplier = ElementsTable[TargetElementIndex][AttackElementIndex];

	// If Attack and Target Element is the same, heals target
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

	GetWorld()->GetTimerManager().SetTimer(CooldownTimer, this, &ABLCombatCharacter::EndCooldown, BaseData.Cooldown, false);
}

void ABLCombatCharacter::EndCooldown()
{
	CurrentDefense = BaseData.BaseDefense;
	bDefendIdle = false;

	TargetCharacters.Empty();

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

	for (const auto* Item : EntriesToDelete)
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
	CurrentAction->ConditionalBeginDestroy();
	CurrentAction = nullptr;
	OnActionEnded.ExecuteIfBound();
}

void ABLCombatCharacter::ReachedActionDestination(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (!IsValid(AIC) || !IsValid(CurrentAction))
	{
		UE_LOG(LogTemp, Warning, TEXT("ABLCombatCharacter::ReachedActionDestination | early return"));
		return;
	}
	
	AIC->GetPathFollowingComponent()->OnRequestFinished.RemoveAll(this);
	AIC->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &ABLCombatCharacter::ReachedSlotLocation);

	CurrentAction->OnEndExecution.BindLambda([this](){ AIC->MoveToLocation(SlotLocation, 5.f); });
	for (ABLCombatCharacter* Target : TargetCharacters)
	{
		CurrentAction->ExecuteAction(this, Target);
	}	
}

void ABLCombatCharacter::ReachedActionDestination(FAIRequestID RequestID, const FPathFollowingResult& Result, int32 TargetIndex)
{
	if (!IsValid(AIC) || !IsValid(CurrentAction))
	{
		return;
	}

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

	CurrentAction->OnEndExecution.BindLambda([this]() { EndAction(true); });	
	CurrentAction->ExecuteAction(this, TargetCharacters[0]);
}

void ABLCombatCharacter::ReachedActionDestination(int32 Index, bool bLastProjectile)
{
	if (!IsValid(CurrentAction) || !TargetCharacters.IsValidIndex(Index) || !TargetCharacters[Index])
	{
		return;
	}

	if (bLastProjectile)
	{
		CurrentAction->OnEndExecution.BindLambda([this]() { EndAction(true); });
	}

	CurrentAction->ExecuteAction(this, TargetCharacters[Index]);
}

void ABLCombatCharacter::ReachedSlotLocation(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (IsValid(AIC))
	{		
		AIC->GetPathFollowingComponent()->OnRequestFinished.RemoveAll(this);
		EndAction(true);
	}
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

void ABLCombatCharacter::GiveStatus(ECombatStatus Status)
{
	Statuses.Add(Status, 3); //TODO: change back to 5 turns or not
	switch (Status)
	{
		case ECombatStatus::BLEEDING:
		{
			BleedingDisplay->SetVisibility(true);
			return;
		}
		case ECombatStatus::POISONING:
		{
			PoisoningDisplay->SetVisibility(true);
			return;
		}
		default: return;
	}
}

void ABLCombatCharacter::RemoveStatus(ECombatStatus Status)
{
	Statuses.Remove(Status);
	switch (Status)
	{
		case ECombatStatus::BLEEDING:
		{
			BleedingDisplay->SetVisibility(false);
			return;
		}
		case ECombatStatus::POISONING:
		{
			PoisoningDisplay->SetVisibility(false);
			return;
		}
		default: return;
	}
}

void ABLCombatCharacter::HandleStatuses()
{
	TArray<ECombatStatus> StatusesToDelete;

	float SimpleDMG = 0.f;

	for (auto& Status : Statuses)
	{
		switch (Status.Key)
		{
			case ECombatStatus::BLEEDING:
			case ECombatStatus::POISONING:
			{
				SimpleDMG += 1.f;
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
	RemoveDel.BindLambda([this, StatusesToDelete]()
	{
		for (const auto& Status : StatusesToDelete)
		{
			RemoveStatus(Status);
		}
	});
	GetWorld()->GetTimerManager().SetTimer(RemoveDelay, RemoveDel, 1.5f, false);
}

void ABLCombatCharacter::TakeSimpleDamage(float Damage)
{
	if (Damage <= 0.f)
	{
		return;
	}

	const float NewHP = FMath::RoundHalfFromZero((CurrentHP - Damage));
	CurrentHP = FMath::Clamp(NewHP, 0, BaseData.MaxHP);
	DisplayTextDMG(Damage, false, ECombatElementType::NONE);

	if (BaseData.TakeDMGAnim)
	{
		GetAnimationComponent()->GetAnimInstance()->PlayAnimationOverride(BaseData.TakeDMGAnim);
	}

	OnHealthUpdate.ExecuteIfBound();
	if (CurrentHP <= 0.f)
	{
		//TODO: add death animation
		//TODO: change idle anim to death
		OnDeath.ExecuteIfBound();
	}
}


void ABLCombatCharacter::HandleHitByAction(ABLCombatCharacter* Attacker, float Damage, ECombatElementType DamageElementType, bool bMagical, const TArray<ECombatStatus>& InStatuses)
{
	bool bIsHeal = false;
	const float DMGMultiplier = CalculateElementsMultipliers(DamageElementType, BaseData.Element, bIsHeal);

	if (bIsHeal)
	{
		const float HealValue = Damage * DMGMultiplier;
		CurrentHP = FMath::Clamp((CurrentHP + HealValue), 0, BaseData.MaxHP);
		DisplayTextDMG(HealValue, true, DamageElementType);
		if (BaseData.HealAnim && BaseData.HealSound)
		{
			GetAnimationComponent()->GetAnimInstance()->PlayAnimationOverride(BaseData.HealAnim);
			UGameplayStatics::PlaySound2D(GetWorld(), BaseData.HealSound);
		}
	}
	else
	{	
		const int32 DodgeChange = FMath::RandRange(1, 100);
		if (DodgeChange <= BaseData.BaseDodge)
		{
			DisplayTextDMG(0, false, DamageElementType, true);
			return;
		}

		// if it draws Pierce, Defense is reduced by half
		const int32 PierceChange = FMath::RandRange(1, 100);
		float NewDefense = PierceChange <= BaseData.Pierce ? CurrentDefense / 2 : CurrentDefense;

		// 10 def decreases dmg by 5%
		float DMGValue = DMGMultiplier > 0 ? ((Damage * DMGMultiplier) * (1.f - ((NewDefense / 1000) * 5))) : 0.f; 

		// if attack is physical and Attacker has Poisoning status, dmg is decreased by 20%
		if (!bMagical && Attacker && Attacker->Statuses.Contains(ECombatStatus::POISONING))
		{
			// Clamp because Defense can be higher than Damage, so that Damage is not negative
			DMGValue = FMath::Clamp(FMath::RoundHalfFromZero(DMGValue * 0.8f), 0, FMath::RoundHalfFromZero(DMGValue));
		}
		else
		{
			DMGValue = FMath::Clamp(FMath::RoundHalfFromZero(DMGValue), 0, FMath::RoundHalfFromZero(DMGValue));
		}

		CurrentHP = FMath::Clamp((CurrentHP - DMGValue), 0, BaseData.MaxHP);
		DisplayTextDMG(DMGValue, false, DamageElementType);
		if (BaseData.TakeDMGAnim && BaseData.TakeDMGSound)
		{
			GetAnimationComponent()->GetAnimInstance()->PlayAnimationOverride(BaseData.TakeDMGAnim);
			UGameplayStatics::PlaySound2D(GetWorld(), BaseData.TakeDMGSound);
		}
	}

	// adding statuses
	for (const auto& Status : InStatuses)
	{
		GiveStatus(Status);
	}

	OnHealthUpdate.ExecuteIfBound();

	if (CurrentHP <= 0.f)
	{
		//TODO: add death animation
		//TODO: change idle anim to death
		OnDeath.ExecuteIfBound();
	}
}



