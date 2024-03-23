// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BLCombatUtilities.h"
#include "BLHeroDataAsset.generated.h"

class UBLAction;

USTRUCT()
struct FHeroAssetInfo 
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FHeroAttributes HeroAttributes;

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

	UPROPERTY(EditAnywhere)
	TArray<TSoftClassPtr<UBLAction>> AttackActions;

	UPROPERTY(EditAnywhere)
	TArray<TSoftClassPtr<UBLAction>> DefendActions;

	// special skills info

	// crystal skills info

	FHeroAssetInfo()
	{
		HeroAttributes = FHeroAttributes();
		Class = nullptr;
		Cooldown = 0.f;
		Element = ECombatElementType::NONE;
		Sprite = nullptr;
		AnimInstanceClass = nullptr;
		TakeDMGAnim = nullptr;
		HealAnim = nullptr;
	}
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
