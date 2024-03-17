// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BLActionWidget.h"
#include "BLDefendActionWidget.generated.h"

/**
 * 
 */
UCLASS()
class BLADEOFLEGEND_API UBLDefendActionWidget : public UBLActionWidget
{
	GENERATED_BODY()

public:
	virtual void SetActionData(const FDefendActionData& DefendData) override;

protected:
	virtual void OnBTActionClicked() override;
};
