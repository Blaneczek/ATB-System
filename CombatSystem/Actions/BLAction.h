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
	virtual void EndAction();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BL|Combat")
	FText Name;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BL|Combat")
	FText Description;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BL|Combat")
	TObjectPtr<UPaperZDAnimSequence> ActionAnim;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BL|Combat")
	ECombatElementType Element;


	FOnEndExecution OnEndExecution;
};
