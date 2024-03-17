// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BLActionWidget.h"
#include "BLAttackActionWidget.generated.h"

/**
 * 
 */
UCLASS()
class BLADEOFLEGEND_API UBLAttackActionWidget : public UBLActionWidget
{
	GENERATED_BODY()

public:
	virtual void SetActionData(const FAttackActionData& AttackData) override;

protected:
	virtual void OnBTActionClicked() override;
};
