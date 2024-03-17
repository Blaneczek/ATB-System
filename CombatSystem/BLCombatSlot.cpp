// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLCombatSlot.h"
#include "Characters/BLCombatCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ABLCombatSlot::ABLCombatSlot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(RootComponent);
	Box->SetBoxExtent(FVector(80.f, 80.f, 32.f));
	Box->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);

	Platform = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform"));
	Platform->SetupAttachment(Box);

	Index = 0;
	bIsEnemy = false;
	bIsActive = false;
	bCanDoAction = false;
	Character = nullptr;
}

// Called when the game starts or when spawned
void ABLCombatSlot::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABLCombatSlot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ABLCombatSlot::GetCooldown() const
{
	return Character->GetCooldown();
}

void ABLCombatSlot::SpawnCharacter(const FCombatCharData& BaseData, const FAttackActionData& AttackData, const FDefendActionData& DefendData)
{
	if (BaseData.Class)
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Character = GetWorld()->SpawnActor<ABLCombatCharacter>(BaseData.Class, GetActorTransform(), SpawnInfo);
		if (Character)
		{
			Character->SetData(BaseData, AttackData, DefendData);
			Character->OnEndCooldown.BindUObject(this, &ABLCombatSlot::EndCharCooldown);
			Character->OnActionEnded.BindUObject(this, &ABLCombatSlot::ActionEnded);
			Character->OnDeath.BindUObject(this, &ABLCombatSlot::HandleCharDeath);
			Character->OnHealthUpdate.BindUObject(this, &ABLCombatSlot::UpdateCharHealth);	
			bIsActive = true;
			// Cooldown will start after 1 sek
			FTimerHandle CooldownTimer;
			GetWorld()->GetTimerManager().SetTimer(CooldownTimer, this, &ABLCombatSlot::StartCharCooldown, 1.f, false);
		}
	}
}

void ABLCombatSlot::PauseCharCooldown()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(Character->GetCooldownTimer()))
	{
		GetWorld()->GetTimerManager().PauseTimer(Character->GetCooldownTimer());
	}
}

void ABLCombatSlot::UnPauseCharCooldown()
{
	if (GetWorld()->GetTimerManager().IsTimerPaused(Character->GetCooldownTimer()))
	{
		GetWorld()->GetTimerManager().UnPauseTimer(Character->GetCooldownTimer());
	}
}

void ABLCombatSlot::DoAction(ECombatActionType ActionType, ABLCombatSlot* TargetSlot)
{
	switch (ActionType)
	{
		case ECombatActionType::ATTACK:
		{
			Character->AttackAction(GetActorLocation(), TargetSlot->GetCharacter());
			return;
		}
		case ECombatActionType::DEFEND:
		{
			Character->DefendAction();
			return;
		}
		case ECombatActionType::CRYSTAL_SKILL:
		{
			//TODO
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

void ABLCombatSlot::EndCharCooldown()
{
	bCanDoAction = true;
	if (bIsEnemy)
	{
		// TODO: something more advanced
		OnEnemyAction.ExecuteIfBound(this, ECombatActionType::ATTACK);
	}
	else
	{
		OnSelectedSlot.ExecuteIfBound(this);
	}
}

void ABLCombatSlot::ActionEnded()
{
	Character->SetActorTransform(GetActorTransform());
	if (bIsActive)
	{
		StartCharCooldown();
		OnCharActionEnded.ExecuteIfBound(this, bIsEnemy);
	}
}

void ABLCombatSlot::StartCharCooldown()
{
	if (Character)
	{
		bCanDoAction = false;
		Character->StartCooldown();
	}
}

void ABLCombatSlot::HandleCharDeath()
{
	bIsActive = false;
	bCanDoAction = false;
	OnCharDeath.ExecuteIfBound(this, bIsEnemy);
}

void ABLCombatSlot::UpdateCharHealth()
{
	OnCharHealthUpdate.ExecuteIfBound(this);
}

