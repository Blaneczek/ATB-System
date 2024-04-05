// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLBackground.h"
#include "BladeOfLegend/AREK/GameInstance/BLGameInstance.h"

ABLBackground::ABLBackground()
{
	PrimaryActorTick.bCanEverTick = false;

	Background = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Background"));
}

void ABLBackground::BeginPlay()
{
	Super::BeginPlay();
	
	UBLGameInstance* GI = Cast<UBLGameInstance>(GetGameInstance());
	if (GI && GI->BackgroundMaterial)
	{
		Background->SetMaterial(0, GI->BackgroundMaterial);
	}

}



