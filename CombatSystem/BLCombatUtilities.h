// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BLCombatUtilities.generated.h"

class UPaperZDAnimInstance;
class UPaperZDAnimSequence;
class UPaperFlipbook;
class ABLCombatCharacter;
class UBLAction;

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

UENUM(BlueprintType)
enum class ECrystalColor : uint8
{
	NONE	UMETA(DisplayName = "None"),
	RED		UMETA(DisplayName = "RED"),
	GREEN	UMETA(DisplayName = "GREEN"),
	BLUE	UMETA(DisplayName = "BLUE")
};

USTRUCT(BlueprintType)
struct FCrystalSkills
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<TSoftClassPtr<UBLAction>> Skills;
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
	int32 BaseHealth;	
	UPROPERTY(EditAnywhere)
	int32 BaseMagicEnergy;
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
		BaseHealth = 100;	
		BaseMagicEnergy = 100;	
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
	float BaseDodge;
	UPROPERTY(EditAnywhere)
	float Cooldown;
	UPROPERTY(EditAnywhere)
	int32 Strength;
	UPROPERTY(EditAnywhere)
	int32 Agility;
	UPROPERTY(EditAnywhere)
	int32 Wisdom;
	UPROPERTY(EditAnywhere)
	int32 Endurance;
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
		MaxHP = 0.f;
		MaxME = 0.f;
		AttackDMG = 0.f;
		BaseDefense = 0.f;
		BaseDodge = 0.f;
		Cooldown = 0.f;
		Strength = 0;
		Agility = 0;
		Wisdom = 0;
		Endurance = 0;
		Element = ECombatElementType::NONE;
		Sprite = nullptr;
		AnimInstanceClass = nullptr;
		TakeDMGAnim = nullptr;
		HealAnim = nullptr;
	}

	FCombatCharData(const FString& InName, TSubclassOf<ABLCombatCharacter> InClass, float InMaxHP
		, float InMaxME, float InAttackDMG, float InBaseDefense, float InBaseDodge
		, float InCooldown, int32 InStrength, int32 InAgility, int32 InWisdom, int32 InEndurance
		, ECombatElementType InElement, UPaperFlipbook* InSprite
		, TSubclassOf<UPaperZDAnimInstance> InAnimClass
		, UPaperZDAnimSequence* InTakeDMGAnim, UPaperZDAnimSequence* InHealAnim)

		:Name(InName), Class(InClass), MaxHP(InMaxHP), MaxME(InMaxME), AttackDMG(InAttackDMG)
		 , BaseDefense(InBaseDefense), BaseDodge(InBaseDodge), Cooldown(InCooldown), Strength(InStrength)
		 , Agility(InAgility), Wisdom(InWisdom), Endurance(InEndurance), Element(InElement)
		 , Sprite(InSprite), AnimInstanceClass(InAnimClass), TakeDMGAnim(InTakeDMGAnim), HealAnim(InHealAnim)
	{}
};
