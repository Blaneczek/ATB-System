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

ABLCombatCharacter::ABLCombatCharacter()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

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

void ABLCombatCharacter::SetData(const FCombatCharData& InBaseData, const FAttackActionData& InAttackData, const FDefendActionData& InDefendData)
{
	bDefendIdle = false;
	BaseData = InBaseData;
	CurrentHP = BaseData.MaxHP;
	CurrentME = BaseData.MaxME;
	CurrentDefense = BaseData.BaseDefense;

	AttackActionData = InAttackData;
	DefendActionData = InDefendData;

	PaperFlipbook->SetFlipbook(BaseData.Sprite);
	GetAnimationComponent()->SetAnimInstanceClass(BaseData.AnimInstanceClass);
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
	OnActionEnded.ExecuteIfBound();
	StartCooldown();
}

void ABLCombatCharacter::ReachedAttackDestination(FAIRequestID RequestID, const FPathFollowingResult& Result)
{	
	UE_LOG(LogTemp, Warning, TEXT("REACHED, %s"));
	
	if (AIC)
	{
		AIC->GetPathFollowingComponent()->OnRequestFinished.RemoveAll(this);
		AIC->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &ABLCombatCharacter::ReachedSlotLocation);
		if (AttackActionData.ActionAnim && TargetCharacter)
		{
			FZDOnAnimationOverrideEndSignature EndAnimDel;
			EndAnimDel.BindLambda([this](bool bResult)
			{
				TargetCharacter->HandleHitByAction(BaseData.AttackDMG, AttackActionData.Element);
				AIC->MoveToLocation(SlotLocation, 3.f);
			});
			GetAnimationComponent()->GetAnimInstance()->PlayAnimationOverride(AttackActionData.ActionAnim, "DefaultSlot", 1.f, 0.0f, EndAnimDel);
		}		
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

	if (CurrentHP <= 0.f)
	{
		//TODO: add death animation
		//TODO: change idle anim to death
		OnDeath.ExecuteIfBound();
	}
}

void ABLCombatCharacter::AttackAction(const FVector& OwnerSlotLocation, ABLCombatCharacter* Target)
{
	TargetCharacter = Target;
	SlotLocation = OwnerSlotLocation;

	if (AttackActionData.bIsRange && ProjectileClass)
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ABLRangeProjectile* Projectile = GetWorld()->SpawnActor<ABLRangeProjectile>(ProjectileClass, GetActorTransform(), SpawnInfo);
		if (Projectile)
		{
			Projectile->SetData(AttackActionData.RangeProjectileSprite, AttackActionData.BaseDMG, AttackActionData.Element);
			Projectile->FlyToTarget(Target);
			Projectile->OnEndProjectile.BindUObject(this, &ABLCombatCharacter::EndAction);
		}
		if (AttackActionData.ActionAnim)
		{
			GetAnimationComponent()->GetAnimInstance()->PlayAnimationOverride(AttackActionData.ActionAnim);
		}	
	}
	else
	{
		if (AIC)
		{	
			AIC->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &ABLCombatCharacter::ReachedAttackDestination);
			UE_LOG(LogTemp, Warning, TEXT("STARTED"));
			AIC->MoveToActor(Target, 70.f);
		}
	}
}

void ABLCombatCharacter::DefendAction()
{
	CurrentDefense = BaseData.BaseDefense * 1.5f;
	bDefendIdle = true;
	FZDOnAnimationOverrideEndSignature EndAnimDel;
	EndAnimDel.BindUObject(this, &ABLCombatCharacter::EndAction);
	if (DefendActionData.ActionAnim)
	{
		GetAnimationComponent()->GetAnimInstance()->PlayAnimationOverride(DefendActionData.ActionAnim, "DefaultSlot", 1.f, 0.0f, EndAnimDel);	
	}
	else
	{
		EndAction(true);
	}
}

void ABLCombatCharacter::SpecialSkillAction()
{
	//TODO
}


