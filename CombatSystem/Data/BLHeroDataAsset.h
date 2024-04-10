// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BLCombatUtilities.h"
#include "BLHeroDataAsset.generated.h"

class UBLAction;

USTRUCT(BlueprintType)
struct FHeroAssetInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FHeroAttributes HeroAttributes;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABLCombatCharacter> Class;

	UPROPERTY(EditAnywhere)
	float Cooldown{0};

	UPROPERTY(EditAnywhere)
	ECombatElementType Element{ECombatElementType::NONE};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UPaperFlipbook> Sprite;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPaperZDAnimInstance> AnimInstanceClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UPaperZDAnimInstance> OutOfCombatAnimInstanceClass;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UPaperZDAnimSequence> TakeDMGAnim;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> TakeDMGSound;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UPaperZDAnimSequence> HealAnim;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> HealSound;

	UPROPERTY(EditAnywhere)
	TArray<TSoftClassPtr<UBLAction>> AttackActions;

	UPROPERTY(EditAnywhere)
	TArray<TSoftClassPtr<UBLAction>> DefendActions;

	UPROPERTY(EditAnywhere)
	TMap<ECrystalColor, FCrystalSkills> CrystalActions;

	UPROPERTY(EditAnywhere)
	TArray<TSoftClassPtr<UBLAction>> SpecialActions;

	//eq info
};

/**
 * 
 */
UCLASS()
class BLADEOFLEGEND_API UBLHeroDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	/** Takes data from the hero (stats) and calculates the variables needed for combat */
	FCombatCharData CalculateBaseCombatData(int32 Index);

public:
	/** Heroes in player's team */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FHeroAssetInfo> Heroes;

	//items info
};
