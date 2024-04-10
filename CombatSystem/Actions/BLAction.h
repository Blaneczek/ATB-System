// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BLActionsInterface.h"
#include "BLCombatUtilities.h"
#include "BLAction.generated.h"

class ABLCombatCharacter;
class UPaperZDAnimSequence;

DECLARE_DELEGATE(FOnEndExecution);

/**
 * 
 */
UCLASS()
class BLADEOFLEGEND_API UBLAction : public UObject
{
	GENERATED_BODY()
	
public:
	virtual void OnCreateAction(ABLCombatCharacter* Owner);
	virtual void ActivateAction(ABLCombatCharacter* Owner) {};
	virtual void ExecuteAction(ABLCombatCharacter* Owner, ABLCombatCharacter* Target) {};

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void ActionCalculations(ABLCombatCharacter* Owner, ABLCombatCharacter* Target);

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BL|Combat")
	FText Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BL|Combat")
	FText Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BL|Combat")
	TObjectPtr<UPaperZDAnimSequence> ActionAnim{ nullptr };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BL|Combat")
	ECombatElementType Element{ ECombatElementType::NONE };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BL|Combat")
	float MECost{ 0.f };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BL|Combat")
	int32 TurnsCost{ 0 };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BL|Combat")
	int32 TargetsNumber{ 1 };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BL|Combat")
	TObjectPtr<USoundBase> ActionSound{ nullptr };

	FOnEndExecution OnEndExecution;
};
