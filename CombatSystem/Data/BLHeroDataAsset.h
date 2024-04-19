// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BLCombatUtilities.h"
#include "BLHeroDataAsset.generated.h"

class UBLAction;
class UBLWeaponItem;
class UBLArmorItem;
class UBLHelmetItem;

USTRUCT(BlueprintType)
struct FHeroAssetInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FHeroAttributes HeroAttributes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABLCombatCharacter> Class;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Cooldown{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECombatElementType Element{ECombatElementType::NONE};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UPaperFlipbook> Sprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UPaperZDAnimInstance> AnimInstanceClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UPaperZDAnimInstance> OutOfCombatAnimInstanceClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UPaperZDAnimSequence> TakeDMGAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USoundBase> TakeDMGSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USoundBase> DeathSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCombatActions CombatActions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UBLWeaponItem> Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UBLArmorItem> Armor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UBLHelmetItem> Helmet;
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
	/** Player's heroes */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FHeroAssetInfo> Heroes;

	//Team's items info
};
