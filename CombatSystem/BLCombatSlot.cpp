// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLCombatSlot.h"
#include "Characters/BLCombatCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/SceneComponent.h"

// Sets default values
ABLCombatSlot::ABLCombatSlot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(RootComponent);
	Box->SetBoxExtent(FVector(80.f, 80.f, 32.f));
	Box->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);

	SpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Helper"));
	SpawnPoint->SetupAttachment(Box);

	TargetPointer = CreateDefaultSubobject<UWidgetComponent>(TEXT("TargetPointer"));
	TargetPointer->SetupAttachment(Box);
	TargetPointer->SetHiddenInGame(true);

	Index = 0;
	bIsEnemy = false;
	bIsActive = false;
	bCanDoAction = false;
	bClicked = false;
	Character = nullptr;
}

// Called when the game starts or when spawned
void ABLCombatSlot::BeginPlay()
{
	Super::BeginPlay();
	
	Box->OnBeginCursorOver.AddDynamic(this, &ABLCombatSlot::OnBeginMouseOver);
	Box->OnEndCursorOver.AddDynamic(this, &ABLCombatSlot::OnEndMouseOver);
}

float ABLCombatSlot::GetCooldown() const
{
	return GetCharacter() ? GetCharacter()->GetCooldown() : 0.f;
}

void ABLCombatSlot::SpawnHero(const FCombatCharData& BaseData, const FCombatActions& CombatActions, bool bSneakAttack)
{
	if (BaseData.Class)
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Character = GetWorld()->SpawnActor<ABLCombatCharacter>(BaseData.Class, SpawnPoint->GetComponentTransform(), SpawnInfo);
		if (Character)
		{
			Character->SetData(BaseData, CombatActions, SpawnPoint->GetComponentTransform());
			Character->OnEndCooldown.BindUObject(this, &ABLCombatSlot::CharCooldownEnded);
			Character->OnActionEnded.BindUObject(this, &ABLCombatSlot::ActionEnded);
			Character->OnDeath.BindUObject(this, &ABLCombatSlot::HandleCharDeath);
			Character->OnHealthUpdate.BindUObject(this, &ABLCombatSlot::UpdateCharHealth);	
			Character->OnEscape.BindUObject(this, &ABLCombatSlot::EscapeCombat);	
			bIsActive = true;

			if (bSneakAttack) Character->SneakAttack();

			// Cooldown will start after 1 sek
			FTimerHandle CooldownTimer;
			GetWorld()->GetTimerManager().SetTimer(CooldownTimer, this, &ABLCombatSlot::StartCharCooldown, 1.f, false);
		}
	}
}

void ABLCombatSlot::SpawnEnemy(const FCombatCharData& BaseData, int32 Level, const TArray<TSoftClassPtr<UBLAction>>& Actions, bool bSneakAttack)
{
	if (BaseData.Class)
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Character = GetWorld()->SpawnActor<ABLCombatCharacter>(BaseData.Class, SpawnPoint->GetComponentTransform(), SpawnInfo);
		if (Character)
		{
			Character->SetData(BaseData, Actions);
			Character->OnEndCooldown.BindUObject(this, &ABLCombatSlot::CharCooldownEnded);
			Character->OnActionEnded.BindUObject(this, &ABLCombatSlot::ActionEnded);
			Character->OnDeath.BindUObject(this, &ABLCombatSlot::HandleCharDeath);
			Character->OnHealthUpdate.BindUObject(this, &ABLCombatSlot::UpdateCharHealth);
			bIsActive = true;

			if (bSneakAttack) Character->SneakAttack();

			// Cooldown will start after 1 sek
			FTimerHandle CooldownTimer;
			GetWorld()->GetTimerManager().SetTimer(CooldownTimer, this, &ABLCombatSlot::StartCharCooldown, 1.f, false);

			OnCharSpawned.ExecuteIfBound(GetIndex(), BaseData.Name, Level, BaseData.Cooldown);
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

void ABLCombatSlot::DoAction(const TArray<ABLCombatSlot*>& TargetsSlots, const FCombatActionData& ActionData, AActor* CombatManager, bool bUseSlots)
{
	if (bUseSlots)
	{
		Character->CreateAction(SpawnPoint->GetComponentLocation(), TargetsSlots, ActionData, CombatManager);
	}
	else
	{
		TArray<ABLCombatCharacter*> Targets;
		for (const auto* Slot : TargetsSlots)
		{
			Targets.Add(Slot->GetCharacter());
		}

		Character->CreateAction(SpawnPoint->GetComponentLocation(), Targets, ActionData, CombatManager);
	}	
}

void ABLCombatSlot::SelectTarget(bool NewSelect)
{
	TargetPointer->SetWidgetClass(TargetWidgetClass);

	if (NewSelect)
	{
		TargetPointer->SetHiddenInGame(false);
		bClicked = true;
	}
	else
	{
		TargetPointer->SetHiddenInGame(true);
		bClicked = false;
	}
}

void ABLCombatSlot::SelectHero(bool NewSelect)
{
	TargetPointer->SetWidgetClass(HeroWidgetClass);

	if (NewSelect)
	{
		TargetPointer->SetHiddenInGame(false);
		bClicked = true;
	}
	else
	{
		TargetPointer->SetHiddenInGame(true);
		bClicked = false;
	}
}

void ABLCombatSlot::HoverMouse(bool NewHover)
{
	if (bClicked) return;

	TargetPointer->SetWidgetClass(HoverWidgetClass);

	if (NewHover)
	{
		TargetPointer->SetHiddenInGame(false);
	}
	else
	{
		TargetPointer->SetHiddenInGame(true);
	}
}

void ABLCombatSlot::DestroyCharacter()
{
	if (GetCharacter())
	{
		GetCharacter()->Destroy();
		OnCharDestroyed.ExecuteIfBound(GetIndex());
	}	
}

void ABLCombatSlot::CharCooldownEnded()
{
	bCanDoAction = true;
	if (IsEnemy())
	{
		// To imitate the thinking process of enemies.
		FTimerDelegate DelayDel;
		FTimerHandle DelayTimer;
		DelayDel.BindWeakLambda(this, [this]() 
			{ 
				OnEnemyAction.ExecuteIfBound(this, GetCharacter()->GetEnemyAction());
			});
		const float RandomDelay = FMath::RandRange(0.5f, 2.5f);
		GetWorld()->GetTimerManager().SetTimer(DelayTimer, DelayDel, RandomDelay, false);	
	}
	else
	{
		OnSelectedSlot.ExecuteIfBound(this);
	}
}

void ABLCombatSlot::ActionEnded()
{
	Character->SetActorTransform(SpawnPoint->GetComponentTransform());
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
	HoverMouse(false);
	OnCharDeath.ExecuteIfBound(this, bIsEnemy);
}

void ABLCombatSlot::UpdateCharHealth()
{
	OnCharHealthUpdate.ExecuteIfBound(this);
}

void ABLCombatSlot::OnBeginMouseOver(UPrimitiveComponent* TouchedComponent)
{
	if (bIsActive)
	{
		HoverMouse(true);
	}
}

void ABLCombatSlot::OnEndMouseOver(UPrimitiveComponent* TouchedComponent)
{
	if (bIsActive)
	{
		HoverMouse(false);
	}
}

void ABLCombatSlot::EscapeCombat(bool bSuccessful)
{
	OnEscapeCombat.ExecuteIfBound(bSuccessful);
}

