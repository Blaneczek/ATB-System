// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BLActionsInterface.generated.h"

class UBLAction;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBLActionsInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BLADEOFLEGEND_API IBLActionsInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	/** Action is executing in place, no targets */
	virtual void DefaultAction() {};

	/** Owner runs up to the target and executes action */
	virtual void DefaultMeleeAction() {};

	/** Owner creates a projectile that flies to the target and executes action */
	virtual void DefaultRangeAction() {};
};
