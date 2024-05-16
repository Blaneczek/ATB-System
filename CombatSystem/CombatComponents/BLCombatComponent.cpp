// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLCombatComponent.h"
#include "BladeOfLegend/Arek/GameInstance/BLGameInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UBLCombatComponent::UBLCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UBLCombatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UBLCombatComponent::StartCombat()
{
	UBLGameInstance* GI = Cast<UBLGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (!GI)
	{
		return;
	}

	// Save the enemy's tag to set his alive/dead flag after the fight 
	if (GetOwner()->Tags.IsValidIndex(0))
	{
		CombatData.EnemyTag = GetOwner()->Tags[0];
	}

	GI->SetCombatData(CombatData, PostCombatData);

	APlayerCameraManager* CM = Cast<APlayerCameraManager>(UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0));
	if (CM)
	{
		CM->StartCameraFade(0.f, 1.f, 1.5f, FLinearColor(0.f, 0.f, 0.f), false, true);
		FTimerDelegate DelayDel;
		DelayDel.BindWeakLambda(this, [this]() { UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), CombatData.CombatMap); });
		FTimerHandle DelayTimer;
		GetWorld()->GetTimerManager().SetTimer(DelayTimer, DelayDel, 2.f, false);
	}
	else
	{
		UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), CombatData.CombatMap);
	}	
}

void UBLCombatComponent::SneakAttack()
{
	CombatData.bSneakAttack = true;
}

void UBLCombatComponent::AddExpAndGold(int32 Exp, int32 Gold)
{
	PostCombatData.Experience += Exp;
	PostCombatData.Gold += Gold;
}


