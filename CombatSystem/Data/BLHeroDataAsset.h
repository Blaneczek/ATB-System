// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BLCombatUtilities.h"
#include "BLHeroDataAsset.generated.h"


USTRUCT()
struct FHeroAssetInfo 
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FHeroAttributes HeroAttributes;

	UPROPERTY(EditAnywhere)
	FAttackActionData AttackAction;

	UPROPERTY(EditAnywhere)
	FDefendActionData DefendAction;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<ABLCombatCharacter> Class;

	UPROPERTY(EditAnywhere)
	float Cooldown;

	UPROPERTY(EditAnywhere)
	ECombatElementType Element;
		
	UPROPERTY(EditAnywhere)
	TObjectPtr<UPaperFlipbook> Sprite;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPaperZDAnimInstance> AnimInstanceClass;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UPaperZDAnimSequence> TakeDMGAnim;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UPaperZDAnimSequence> HealAnim;

	// special skills info

	// crystal skills info
};

/**
 * 
 */
UCLASS()
class BLADEOFLEGEND_API UBLHeroDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	FCombatCharData CalculateBaseCombatData(int32 Index);

public:
	UPROPERTY(EditAnywhere)
	TArray<FHeroAssetInfo> Heroes;

	//items info
};
