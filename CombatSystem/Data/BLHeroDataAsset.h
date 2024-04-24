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

	// To auto calculate CurrentHP, test it more in future
	void UpdateCurrentHP();
	void UpdateCurrentME();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FHeroAttributes HeroAttributes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABLCombatCharacter> Class = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText SpecialActionsName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Cooldown = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECombatElementType Element = ECombatElementType::NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UPaperFlipbook> Sprite = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UPaperZDAnimInstance> AnimInstanceClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UPaperZDAnimInstance> OutOfCombatAnimInstanceClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UPaperZDAnimSequence> TakeDMGAnim = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USoundBase> TakeDMGSound = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USoundBase> DeathSound = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCombatActions CombatActions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UBLWeaponItem> Weapon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UBLArmorItem> Armor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UBLHelmetItem> Helmet = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<UBLItem>> Items;
};

/**
 * 
 */
UCLASS(Blueprintable)
class BLADEOFLEGEND_API UBLHeroDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	// To auto calculate CurrentHP, test it more in future
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	
	/** Takes data from the hero (stats) and calculates the variables needed for combat */
	FCombatCharData CalculateBaseCombatData(int32 Index);

	/** Adds to data asset reward from won combat */
	void AddCombatReward(int32 InExperience, int32 InMoney, const TArray<TSoftClassPtr<UBLItem>>& InItems);

	UFUNCTION(BlueprintCallable)
	float GetMaxHP(int32 Index) const;

	UFUNCTION(BlueprintCallable)
	float GetMaxME(int32 Index) const;

public:
	/** Player's heroes */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FHeroAssetInfo> Heroes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSoftClassPtr<UBLItem>> Items;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Money;
};
