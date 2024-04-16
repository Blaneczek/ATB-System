// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BLLostCombatWidget.generated.h"

class UButton;
class UTextBlock;

DECLARE_DELEGATE(FOnLostGame);

/**
 *
 */
UCLASS()
class BLADEOFLEGEND_API UBLLostCombatWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void OnBTNextClicked();

public:
	FOnLostGame OnLostGame;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat", meta = (BindWidget))
	TObjectPtr<UButton> BTNext;
};
