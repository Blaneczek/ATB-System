// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BLWindowDisplayInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UBLWindowDisplayInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BLADEOFLEGEND_API IBLWindowDisplayInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ShowQuestText();
};
