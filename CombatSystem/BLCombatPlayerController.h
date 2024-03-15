// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BLCombatPlayerController.generated.h"

class ABLCombatCamera;
class UInputMappingContext;
class UInputAction;

DECLARE_DELEGATE_OneParam(FOnSlotClicked, AActor* /*ClickedActor*/);

/**
 * 
 */
UCLASS()
class BLADEOFLEGEND_API ABLCombatPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected: 
	virtual void BeginPlay() override;

public:
	virtual void SetupInputComponent() override;

private:
	UFUNCTION()
	void MouseClick();

public:
	FOnSlotClicked OnSlotClicked;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BL|Combat")
	TObjectPtr<UInputMappingContext> MappingContext;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BL|Combat")
	TObjectPtr<UInputAction> MouseClickAction;

private:
	UPROPERTY(EditDefaultsOnly, Category = "BL|Combat")
	TSubclassOf<ABLCombatCamera> CameraClass;
};
