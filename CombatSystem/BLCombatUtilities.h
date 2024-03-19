// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BLCombatUtilities.generated.h"

class UPaperZDAnimInstance;
class UPaperZDAnimSequence;
class UPaperFlipbook;
class ABLRangeProjectile;
class ABLCombatCharacter;

UENUM(BlueprintType)
enum class ECombatActionType : uint8
{
	NONE			UMETA(DisplayName = "None"),
	ATTACK			UMETA(DisplayName = "Attack"),
	DEFEND			UMETA(DisplayName = "Defend"),
	CRYSTAL_SKILL	UMETA(DisplayName = "Crystal skill"),
	SPECIAL_SKILL	UMETA(DisplayName = "Special skill"),
	ITEM			UMETA(DisplayName = "Item"),
	RUN_AWAY		UMETA(DisplayName = "Run away")
};

UENUM(BlueprintType)
enum class ECombatElementType : uint8
{
	FIRE	UMETA(DisplayName = "Fire"),
	WATER	UMETA(DisplayName = "Water"),
	EARTH	UMETA(DisplayName = "Earth"),
	WIND	UMETA(DisplayName = "Wind"),
	ICE		UMETA(DisplayName = "Ice"),
	THUNDER UMETA(DisplayName = "Thunder"),
	ACID	UMETA(DisplayName = "Acid"),
	DARK	UMETA(DisplayName = "Dark"),
	WHITE	UMETA(DisplayName = "White"),
	NONE	UMETA(DisplayName = "None")
};

USTRUCT(BlueprintType)
struct FHeroAttributes
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FString Name;
	UPROPERTY(EditAnywhere)
	uint32 Level;
	UPROPERTY(EditAnywhere)
	uint32 Experience;
	UPROPERTY(EditAnywhere)
	uint32 ExperienceNextLevel;
	UPROPERTY(EditAnywhere)
	int32 MaxHealth;
	UPROPERTY(EditAnywhere)
	int32 CurrentHealth;
	UPROPERTY(EditAnywhere)
	int32 MaxMagicEnergy;
	UPROPERTY(EditAnywhere)
	int32 CurrentMagicEnergy;
	UPROPERTY(EditAnywhere)
	int32 Strength;
	UPROPERTY(EditAnywhere)
	int32 Agility;
	UPROPERTY(EditAnywhere)
	int32 Wisdom;
	UPROPERTY(EditAnywhere)
	int32 Endurance;

	FHeroAttributes()
	{
		Name = "";
		Level = 1;
		Experience = 0;
		ExperienceNextLevel = 100;
		MaxHealth = 100;
		CurrentHealth = MaxHealth;
		MaxMagicEnergy = 100;
		CurrentMagicEnergy = MaxMagicEnergy;
		Strength = 5;
		Agility = 5;
		Wisdom = 5;
		Endurance = 5;
	}
};

USTRUCT(BlueprintType)
struct FCombatCharData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FString Name;
	UPROPERTY(EditAnywhere)
	TSubclassOf<ABLCombatCharacter> Class;
	UPROPERTY(EditAnywhere)
	float MaxHP;
	UPROPERTY(EditAnywhere)
	float MaxME;
	UPROPERTY(EditAnywhere)
	float AttackDMG;
	UPROPERTY(EditAnywhere)
	float BaseDefense;
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

	FCombatCharData()
	{
		Name = "";
		Class = nullptr;
		MaxHP = 0;
		MaxME = 0;
		AttackDMG = 0;
		BaseDefense = 0;
		Cooldown = 0;
		Element = ECombatElementType::NONE;
		Sprite = nullptr;
		AnimInstanceClass = nullptr;
		TakeDMGAnim = nullptr;
		HealAnim = nullptr;
	}

	FCombatCharData(const FString& InName, TSubclassOf<ABLCombatCharacter> InClass, float InMaxHP,
		float InMaxME, float InAttackDMG, float InBaseDefense,
		float InCooldown, ECombatElementType InElement,
		UPaperFlipbook* InSprite,
		TSubclassOf<UPaperZDAnimInstance> InAnimClass,
		UPaperZDAnimSequence* InTakeDMGAnim, UPaperZDAnimSequence* InHealAnim)
		:Name(InName), Class(InClass), MaxHP(InMaxHP), MaxME(InMaxME), AttackDMG(InAttackDMG),
		BaseDefense(InBaseDefense), Cooldown(InCooldown), Element(InElement),
		Sprite(InSprite), AnimInstanceClass(InAnimClass), TakeDMGAnim(InTakeDMGAnim), HealAnim(InHealAnim)
	{}
};

USTRUCT(BlueprintType)
struct FAttackActionData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FText Name;
	UPROPERTY(EditAnywhere)
	FText Description;
	UPROPERTY(EditAnywhere)
	ECombatElementType Element;
	UPROPERTY(EditAnywhere)
	bool bIsRange;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UPaperZDAnimSequence> ActionAnim;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UPaperFlipbook> RangeProjectileSprite;

	FAttackActionData()
	{
		Name = FText::FromString("");
		Description = FText::FromString("");
		Element = ECombatElementType::NONE;
		bIsRange = false;
		ActionAnim = nullptr;
		RangeProjectileSprite = nullptr;
	}

	FAttackActionData(const FText& InName, const FText& InDesc, ECombatElementType InElement, bool IsRange, UPaperZDAnimSequence* InActionAnim, UPaperFlipbook* InProjectileSprite)
		:Name(InName), Description(InDesc), Element(InElement), bIsRange(bIsRange), ActionAnim(InActionAnim), RangeProjectileSprite(InProjectileSprite)
	{}
};

USTRUCT(BlueprintType)
struct FDefendActionData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FText Name;
	UPROPERTY(EditAnywhere)
	FText Description;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UPaperZDAnimSequence> ActionAnim;

	FDefendActionData()
	{
		Name = FText::FromString("");
		Description = FText::FromString("");
		ActionAnim = nullptr;
	}

	FDefendActionData(const FText& InName, const FText& InDesc, UPaperZDAnimSequence* InActionAnim)
		:Name(InName), Description(InDesc), ActionAnim(InActionAnim)
	{}
};