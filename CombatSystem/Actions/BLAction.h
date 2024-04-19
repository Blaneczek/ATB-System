// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BLActionsInterface.h"
#include "BLCombatUtilities.h"
#include "BLAction.generated.h"

class ABLCombatCharacter;
class UPaperZDAnimSequence;
class ABLCombatManager;

DECLARE_DELEGATE(FOnEndExecution);

/**
 * 
 */
UCLASS()
class BLADEOFLEGEND_API UBLAction : public UObject
{
	GENERATED_BODY()
	
public:
	virtual void OnCreateAction(ABLCombatCharacter* Owner, AActor* InCombatManager);
	virtual void ActivateAction(ABLCombatCharacter* Owner) {};
	virtual void ExecuteAction(ABLCombatCharacter* Owner, ABLCombatCharacter* Target) {};

protected:
	/** Use in blueprints. Each action is of a different class, so each action may have a different use when executed */
	UFUNCTION(BlueprintImplementableEvent)
	void ActionCalculations(ABLCombatCharacter* Owner, ABLCombatCharacter* Target, AActor* InCombatManager);

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BL|Combat")
	FText Name;

	/** Text displayed in the combat Widget  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BL|Combat")
	FText Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BL|Combat")
	TObjectPtr<UPaperZDAnimSequence> ActionAnim = nullptr;

	/** Needed to correctly select slots in the action, e.g. the action can only be used on the owner */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BL|Combat")
	ECombatActionFlow Flow = ECombatActionFlow::NONE;

	/** If the action is magical, this CombatElement will be used */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BL|Combat")
	ECombatElementType Element = ECombatElementType::NONE;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BL|Combat")
	bool bMagical = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BL|Combat")
	float MECost = 0.f;

	/** How many character's turns must pass before the next use of a given action */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BL|Combat")
	int32 TurnsCost = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BL|Combat")
	int32 TargetsNumber = 1;

	/** To call interface functions in CombatManager */
	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat")
	TObjectPtr<AActor> CombatManager;

	/** Called when the action is completed */
	FOnEndExecution OnEndExecution;
};
