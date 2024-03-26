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

ABLCombatCharacter::ABLCombatCharacter()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	AIC = nullptr;
	TargetCharacter = nullptr;
	CurrentAction = nullptr;
	Movement->MaxWalkSpeed = 600.f;
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

void ABLCombatCharacter::SetData(const FCombatCharData& InBaseData, const TArray<TSoftClassPtr<UBLAction>>& InAttackActions, const TArray<TSoftClassPtr<UBLAction>>& InDefendActions, const TMap<ECrystalColor, FCrystalSkills>& InCrystalActions)
{
	SetData(InBaseData, InAttackActions, InDefendActions);

	CrystalActions = InCrystalActions;
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


void ABLCombatCharacter::CreateAction(const FVector& OwnerSlotLocation, ECombatActionType ActionType, int32 ActionIndex, ABLCombatCharacter* Target, ECrystalColor CrystalColor)
{
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
			TargetCharacter = Target;

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
			TargetCharacter = Target;

			CurrentAction = NewObject<UBLAction>(this, CrystalActions.Find(CrystalColor)->Skills[ActionIndex].LoadSynchronous());
			if (CurrentAction)
			{
				UE_LOG(LogTemp, Warning, TEXT("%s"), *CurrentAction->GetName());
				CurrentAction->OnCreateAction(this);
			}
			return;
		}
		case ECombatActionType::SPECIAL_SKILL:
		{
			//TODO
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
	if (AIC)
	{
		AIC->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &ABLCombatCharacter::ReachedActionDestination);
		UE_LOG(LogTemp, Warning, TEXT("STARTED"));
		AIC->MoveToActor(TargetCharacter, 70.f);
	}
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
			Projectile->FlyToTarget(TargetCharacter);
		}
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
	UE_LOG(LogTemp, Warning, TEXT("REACHED"));
	
	if (AIC)
	{
		AIC->GetPathFollowingComponent()->OnRequestFinished.RemoveAll(this);
		AIC->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &ABLCombatCharacter::ReachedSlotLocation);

		if (CurrentAction)
		{
			CurrentAction->OnEndExecution.BindLambda([this](){ AIC->MoveToLocation(SlotLocation, 10.f); });
			CurrentAction->ExecuteAction(this, TargetCharacter);		
		}
		else
		{
			AIC->MoveToLocation(SlotLocation, 10.f);
		}	
	}
}

void ABLCombatCharacter::ReachedActionDestination()
{
	if (CurrentAction)
	{
		UE_LOG(LogTemp, Warning, TEXT("REACHED projectile"));	
		CurrentAction->OnEndExecution.BindLambda([this]() { EndAction(true); });
		CurrentAction->ExecuteAction(this, TargetCharacter);
	}
}

void ABLCombatCharacter::ReachedSlotLocation(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (AIC)
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
		//TODO: display heal value
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
		//TODO: display dmg value
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



