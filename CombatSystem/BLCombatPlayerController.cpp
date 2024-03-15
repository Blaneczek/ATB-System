// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLCombatPlayerController.h"
#include "BLCombatCamera.h"
#include "Kismet/GameplayStatics.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void ABLCombatPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (CameraClass)
	{
		AActor* Camera = UGameplayStatics::GetActorOfClass(GetWorld(), CameraClass);
		if (Camera)
		{
			SetViewTarget(Camera);
		}
	}	

	SetShowMouseCursor(true);
	bEnableClickEvents = true;

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(MappingContext, 0);
	}
}

void ABLCombatPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MouseClickAction, ETriggerEvent::Triggered, this, &ABLCombatPlayerController::MouseClick);
	}
}

void ABLCombatPlayerController::MouseClick()
{
	FHitResult HitResult;
	const bool bResult = GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1), true, HitResult);
	if (bResult)
	{
		OnSlotClicked.ExecuteIfBound(HitResult.GetActor());
	}
}
