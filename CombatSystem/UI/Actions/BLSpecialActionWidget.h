// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BLActionWidget.h"
#include "BLSpecialActionWidget.generated.h"

/**
 * 
 */
UCLASS()
class BLADEOFLEGEND_API UBLSpecialActionWidget : public UBLActionWidget
{
	GENERATED_BODY()

public:
	virtual void AddActions(const TArray<TSoftClassPtr<UBLAction>>& InActions) override;

protected:
	virtual void OnActionClicked(UObject* Item) override;

public:
	UPROPERTY()
	int32 TurnsCost { 0 };
};
