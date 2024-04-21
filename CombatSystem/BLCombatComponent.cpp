// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLCombatComponent.h"
#include "BladeOfLegend/Arek/GameInstance/BLGameInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UBLCombatComponent::UBLCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UBLCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UBLCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UBLCombatComponent::StartCombat()
{
	UBLGameInstance* GI = Cast<UBLGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI)
	{
		GI->SetCombatData(CombatData, PostCombatData);
	}

	APlayerCameraManager* CM = Cast<APlayerCameraManager>(UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0));
	if (CM)
	{
		CM->StartCameraFade(0.f, 1.f, 1.5f, FLinearColor(0.f, 0.f, 0.f), false, true);
		FTimerDelegate DelayDel;
		DelayDel.BindLambda([this]() { UGameplayStatics::OpenLevel(GetWorld(), "BLCombat"); });
		FTimerHandle DelayTimer;
		GetWorld()->GetTimerManager().SetTimer(DelayTimer, DelayDel, 2.f, false);
	}
	else
	{
		UGameplayStatics::OpenLevel(GetWorld(), "BLCombat");
	}	
}

void UBLCombatComponent::SneakAttack()
{
	CombatData.bSneakAttack = true;
}


