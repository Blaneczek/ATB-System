// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BLCombatUtilities.generated.h"

class UPaperZDAnimInstance;
class UPaperZDAnimSequence;
class UPaperFlipbook;
class ABLCombatCharacter;
class UBLAction;
class ULevelSequence;
class UBLEnemyDataAsset;

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
enum class ECombatActionFlow : uint8
{
	NONE			UMETA(DisplayName = "None"),
	DEFAULT			UMETA(DisplayName = "Default"),
	DEFAULT_MELEE	UMETA(DisplayName = "Default melee"),
	DEFAULT_RANGE	UMETA(DisplayName = "Default range"),
	MULTIPLE_MELEE	UMETA(DisplayName = "Multiple melee"),
	MULTIPLE_RANGE	UMETA(DisplayName = "Multiple range"),
	COLUMN_MELEE	UMETA(DisplayName = "Column melee")
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

UENUM(BlueprintType)
enum class ECombatStatus : uint8
{
	NONE		UMETA(DisplayName = "None"),
	BLEEDING	UMETA(DisplayName = "Bleeding"),
	POISONING	UMETA(DisplayName = "Poisoning"),
	BLOODLUST	UMETA(DisplayName = "Bloodlust"),
	STUN		UMETA(DisplayName = "Stun"),
	BLINDING	UMETA(DisplayName = "Blinding"),
	SPEEDUP		UMETA(DisplayName = "Speedup"),
	INSPIRATION	UMETA(DisplayName = "Inspiration"),
	FLAMING		UMETA(DisplayName = "Flaming")
};

USTRUCT(BlueprintType)
struct FCrystalSkills
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<TSoftClassPtr<UBLAction>> Skills;
};

USTRUCT(BlueprintType)
struct FCombatItems
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UBLAction> Action;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> Thumbnail;
};

USTRUCT(BlueprintType)
struct FHeroAttributes
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FString Name;
	UPROPERTY(EditAnywhere)
	int32 Level;
	UPROPERTY(EditAnywhere)
	int32 Experience;
	UPROPERTY(EditAnywhere)
	int32 ExperienceNextLevel;
	UPROPERTY(EditAnywhere)
	float BaseHealth;	
	UPROPERTY(EditAnywhere)
	float BaseMagicEnergy;
	UPROPERTY(EditAnywhere)
	int32 Strength;
	UPROPERTY(EditAnywhere)
	int32 Agility;
	UPROPERTY(EditAnywhere)
	int32 Wisdom;
	UPROPERTY(EditAnywhere)
	int32 Endurance;
	UPROPERTY(EditAnywhere)
	int32 TotalCrystalsLevel;

	FHeroAttributes()
		: Name(""), Level(1), Experience(0), ExperienceNextLevel(100), BaseHealth(20.f)
		, BaseMagicEnergy(10.f), Strength(1), Agility(1), Wisdom(1), Endurance(1), TotalCrystalsLevel(0)
	{}
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
	float BaseAttackDMG;
	UPROPERTY(EditAnywhere)
	int32 BaseDefense;
	UPROPERTY(EditAnywhere)
	int32 BaseDodge;
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
	int32 Pierce;
	UPROPERTY(EditAnywhere)
	ECombatElementType Element;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UPaperFlipbook> Sprite;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UPaperZDAnimInstance> AnimInstanceClass;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UPaperZDAnimSequence> TakeDMGAnim;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> TakeDMGSound;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UPaperZDAnimSequence> HealAnim;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> HealSound;

	FCombatCharData()
		:Name(""), Class(nullptr), MaxHP(0.f), MaxME(0.f), BaseAttackDMG(0.f)
		, BaseDefense(0), BaseDodge(0), Cooldown(0.f), Strength(0)
		, Agility(0), Wisdom(0), Endurance(0), Pierce(0), Element(ECombatElementType::NONE)
		, Sprite(nullptr), AnimInstanceClass(nullptr), TakeDMGAnim(nullptr)
		, TakeDMGSound(nullptr), HealAnim(nullptr), HealSound(nullptr)
	{}

	FCombatCharData(const FString& InName, TSubclassOf<ABLCombatCharacter> InClass, float InMaxHP
		, float InMaxME, float InAttackDMG, int32 InBaseDefense, int32 InBaseDodge
		, float InCooldown, int32 InStrength, int32 InAgility, int32 InWisdom, int32 InEndurance
		, int32 InPierce, ECombatElementType InElement, UPaperFlipbook* InSprite
		, TSubclassOf<UPaperZDAnimInstance> InAnimClass
		, UPaperZDAnimSequence* InTakeDMGAnim, USoundBase* InTakeDMGSound
		, UPaperZDAnimSequence* InHealAnim, USoundBase* InHealSound)

		:Name(InName), Class(InClass), MaxHP(InMaxHP), MaxME(InMaxME), BaseAttackDMG(InAttackDMG)
		, BaseDefense(InBaseDefense), BaseDodge(InBaseDodge), Cooldown(InCooldown), Strength(InStrength)
		, Agility(InAgility), Wisdom(InWisdom), Endurance(InEndurance), Pierce(InPierce), Element(InElement)
		, Sprite(InSprite), AnimInstanceClass(InAnimClass), TakeDMGAnim(InTakeDMGAnim)
		, TakeDMGSound(InTakeDMGSound), HealAnim(InHealAnim), HealSound(InHealSound)
	{}
};

USTRUCT(BlueprintType)
struct FCombatActionData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	ECombatActionType Type;

	UPROPERTY(EditAnywhere)
	ECombatActionFlow Flow;

	UPROPERTY(EditAnywhere)
	int32 Index;

	UPROPERTY(EditAnywhere)
	ECrystalColor CrystalColor;

	UPROPERTY(EditAnywhere)
	float MECost;

	UPROPERTY(EditAnywhere)
	int32 TargetsNum;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UObject> ActionEntry;

	FCombatActionData()
		: Type(ECombatActionType::NONE), Flow(ECombatActionFlow::NONE), Index(0), CrystalColor(ECrystalColor::NONE)
		, MECost(0.f), TargetsNum(1), ActionEntry(nullptr)
	{}

	FCombatActionData(ECombatActionType InType, ECombatActionFlow InFlow, int32 InIndex
		, ECrystalColor InCrystalColor = ECrystalColor::NONE, float InMECost = 0.f, int32 InTargetsNum = 1.f, UObject* InActionEntry = nullptr)

		: Type(InType), Flow(InFlow), Index(InIndex), CrystalColor(InCrystalColor)
		, MECost(InMECost), TargetsNum(InTargetsNum), ActionEntry(InActionEntry)
	{}
};

USTRUCT(BlueprintType)
struct FCombatActions
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<TSoftClassPtr<UBLAction>> AttackActions;

	UPROPERTY(EditAnywhere)
	TArray<TSoftClassPtr<UBLAction>> DefendActions;

	UPROPERTY(EditAnywhere)
	TMap<ECrystalColor, FCrystalSkills> CrystalActions;

	UPROPERTY(EditAnywhere)
	TArray<TSoftClassPtr<UBLAction>> SpecialActions;

	UPROPERTY(EditAnywhere)
	TArray<FCombatItems> ItemActions;

	FCombatActions()
	{}

	FCombatActions(const TArray<TSoftClassPtr<UBLAction>>& InAttackActions, const TArray<TSoftClassPtr<UBLAction>>& InDefendActions
		, const TMap<ECrystalColor, FCrystalSkills>& InCrystalActions, const TArray<TSoftClassPtr<UBLAction>>& InSpecialActions
		, const TArray<FCombatItems>& InItemActions)

		: AttackActions(InAttackActions), DefendActions(InDefendActions), CrystalActions(InCrystalActions)
		, SpecialActions(InSpecialActions), ItemActions(InItemActions)
	{}
};

USTRUCT(BlueprintType)
struct FPostCombatData
{
	GENERATED_BODY()

	/** Level to open after the fight */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName LevelName;

	/** Experience gained after winning fight */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Experience;

	/** Money gained after winnging fight */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Money;

	/** Optional level sequence to play after the fight */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<ULevelSequence> LevelSequence;

	/** Optional player's position if he was in Overworld before the fight */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector PlayerPosition;

	/** Optional items gained after winning fight */
	// TODO: items

	FPostCombatData()
		: LevelName(""), Experience(0), Money(0), LevelSequence(nullptr), PlayerPosition(FVector(0.f, 0.f, 0.f))
	{}

	FPostCombatData(const FName& InName, int32 InExperience, int32 InMoney, ULevelSequence* InLevelSequence = nullptr, const FVector& InPlayerPosition = FVector(0.f, 0.f, 0.f))
		: LevelName(InName), Experience(InExperience), Money(InMoney), LevelSequence(InLevelSequence), PlayerPosition(InPlayerPosition)
	{}
};

USTRUCT(BlueprintType)
struct FCombatData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UBLEnemyDataAsset> EnemyData;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UMaterialInstance> BackgroundMaterial;

	/** Optional */
	UPROPERTY(EditAnywhere)
	TArray<FText> QuestDisplayTexts;

	FCombatData()
		: EnemyData(nullptr), BackgroundMaterial(nullptr)
	{}

	FCombatData(TSoftObjectPtr<UBLEnemyDataAsset> InEnemyData, UMaterialInstance* InBackgroundMaterial, const TArray<FText>& InQuestDisplayTexts)
		: EnemyData(InEnemyData), BackgroundMaterial(InBackgroundMaterial), QuestDisplayTexts(InQuestDisplayTexts)
	{}
};

USTRUCT(BlueprintType)
struct FCombatStatus
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECombatStatus Status{ ECombatStatus::NONE };

	/** Number of cooldowns for which the status will be active */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Turns{ 0 };

	/** Percentage chance of status application. 0 - 0%, 100 - 100% */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ApplicationChance{ 0 };
};