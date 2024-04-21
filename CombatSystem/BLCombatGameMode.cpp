// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLCombatGameMode.h"
#include "Kismet/GameplayStatics.h"

void ABLCombatGameMode::BeginPlay()
{
	Super::BeginPlay();

	APlayerCameraManager* CM = Cast<APlayerCameraManager>(UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0));
	if (CM)
	{
		CM->StartCameraFade(1.f, 0.f, 1.f, FLinearColor(0.f, 0.f, 0.f), false, true);
	}
}
