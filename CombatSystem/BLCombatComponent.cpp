// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLCombatComponent.h"
#include "BladeOfLegend/Arek/GameInstance/BLGameInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UBLCombatComponent::UBLCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

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

	UGameplayStatics::OpenLevel(GetWorld(), "BLCombat");
}


