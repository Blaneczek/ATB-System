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
#include "Components/WidgetComponent.h"
#include "UI/Entries/BLActionEntryData.h"
#include "Kismet/KismetMathLibrary.h"
#include "Core/BLCombatSlot.h"
#include "CombatComponents/BLActionComponent.h"
#include "CombatComponents/BLStatusesComponent.h"

ABLCombatCharacter::ABLCombatCharacter()
{
	DMGDisplay = CreateDefaultSubobject<UWidgetComponent>(TEXT("DMG Display"));
	DMGDisplay->SetupAttachment(PaperFlipbook);

	StatusDisplay = CreateDefaultSubobject<UWidgetComponent>(TEXT("Bleeding Display"));
	StatusDisplay->SetupAttachment(PaperFlipbook);

	ActionComponent = CreateDefaultSubobject<UBLActionComponent>(TEXT("Action manager"));
	StatusesComponent = CreateDefaultSubobject<UBLStatusesComponent>(TEXT("Statuses manager"));

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bDead = false;
	bMagicImmunity = false;
	bDefendIdle = false;
	Movement->MaxWalkSpeed = 900.f;
}

void ABLCombatCharacter::BeginPlay()
{
	Super::BeginPlay();

	ActionComponent->OnActionFinished.BindWeakLambda(this, [this]() { OnActionEnded.ExecuteIfBound(); });
	ActionComponent->OnEscapeAction.BindWeakLambda(this, [this](bool bSuccessful) {OnEscape.ExecuteIfBound(bSuccessful); });
}

void ABLCombatCharacter::SetData(const FCombatCharData& InBaseData)
{
	BaseData = InBaseData;
	CurrentHP = BaseData.CurrentHP;
	CurrentME = BaseData.MaxME;
	CurrentDefense = BaseData.BaseDefense;
	CurrentCooldown = BaseData.Cooldown;
	CurrentDodge = BaseData.BaseDodge;
	CurrentAttackDMG = BaseData.BaseAttackDMG;

	PaperFlipbook->SetFlipbook(BaseData.Sprite);
	
	// PURE VIRTUAL FUNCTION CALL BUG
	//GetAnimationComponent()->SetAnimInstanceClass(BaseData.AnimInstanceClass);
}

void ABLCombatCharacter::SetHeroData(const FCombatCharData& InBaseData, const FCombatActions& InCombatActions)
{
	SetData(InBaseData);

	ActionComponent->SetActions(InCombatActions);	
}


void ABLCombatCharacter::SetEnemyData(const FCombatCharData& InBaseData, const TArray<TSoftClassPtr<UBLAction>>& InActions)
{
	SetData(InBaseData);

	// For enemies to choose actions
	AttackActions = InActions;
	ActionComponent->SetActions(InActions);
}

void ABLCombatCharacter::CreateAction(const FVector& OwnerSlotLocation, const TArray<ABLCombatSlot*>& Targets, const FCombatActionData& ActionData, AActor* CombatManager)
{
	ClickedActionEntry = Cast<UBLActionEntryData>(ActionData.ActionEntry);
	ActionComponent->CreateAction(OwnerSlotLocation, Targets, ActionData, CombatManager);
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
	StatusesComponent->HandleStatuses();

	GetWorld()->GetTimerManager().SetTimer(CooldownTimer, this, &ABLCombatCharacter::EndCooldown, CurrentCooldown, false);
}

void ABLCombatCharacter::EndCooldown()
{
	HandleTurnsCooldown();

	OnEndCooldown.ExecuteIfBound();
}

void ABLCombatCharacter::GiveStatus(ECombatStatusType Status, int32 Turns)
{
	StatusesComponent->GiveStatus(Status, Turns);
}

void ABLCombatCharacter::RemoveStatus(ECombatStatusType Status)
{
	StatusesComponent->RemoveStatus(Status);
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

void ABLCombatCharacter::HandleHealHit(float Heal, float HealMultiplier, ECombatElementType HealElementType)
{
	const float HealValue = Heal * HealMultiplier;
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
	if (!bMagicalAction && Attacker->StatusesComponent->Statuses.Contains(ECombatStatusType::POISONING))
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

	StatusesComponent->HandleHitStatuses(Attacker, InStatuses, bMagical);

	if (CurrentHP <= 0.f)
	{
		bDead = true;
		StatusDisplay->SetVisibility(false);
		OnDeath.ExecuteIfBound();
	}
}



