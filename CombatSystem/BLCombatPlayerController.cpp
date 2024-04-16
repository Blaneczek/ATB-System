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
	bEnableMouseOverEvents = true;

	SetInputMode(FInputModeGameAndUI());

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
		EnhancedInputComponent->BindAction(MouseLeftClickAction, ETriggerEvent::Triggered, this, &ABLCombatPlayerController::MouseLeftClick);
		EnhancedInputComponent->BindAction(MouseRightClickAction, ETriggerEvent::Triggered, this, &ABLCombatPlayerController::MouseRightClick);
	}
}

void ABLCombatPlayerController::MouseRightClick()
{
	OnSlotRemoved.ExecuteIfBound();
}

void ABLCombatPlayerController::MouseLeftClick()
{
	FHitResult HitResult;
	const bool bResult = GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1), true, HitResult);
	if (bResult)
	{
		OnSlotClicked.ExecuteIfBound(HitResult.GetActor());
	}
}
