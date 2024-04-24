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
class UBLItem;
class UBLCombatItem;

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
	PURPLE	UMETA(DisplayName = "PURPLE"),
	WHITE	UMETA(DisplayName = "WHITE")
};

UENUM(BlueprintType)
enum class ECombatStatusType : uint8
{
	NONE		UMETA(DisplayName = "None"),
	BLEEDING	UMETA(DisplayName = "Bleeding"),
	POISONING	UMETA(DisplayName = "Poisoning"),
	BLOODLUST	UMETA(DisplayName = "Bloodlust"),
	STUNNED		UMETA(DisplayName = "Stunned"),
	BLINDING	UMETA(DisplayName = "Blinding"),
	SPEEDUP		UMETA(DisplayName = "Speedup"),
	INSPIRATION	UMETA(DisplayName = "Inspiration"),
	FLAMING		UMETA(DisplayName = "Flaming"),
	SHIELD		UMETA(DisplayName = "Shield"),
	SNEAK		UMETA(DisplayName = "Sneak attack")
};


UENUM(BlueprintType)
enum class EItemType: uint8
{
	NONE		UMETA(DisplayName = "None"),
	WEAPON		UMETA(DisplayName = "Weapon"),
	ARMOR		UMETA(DisplayName = "Armor"),
	HELMET		UMETA(DisplayName = "Helmet"),
	USABLE		UMETA(DisplayName = "Helmet"),
	JUNK		UMETA(DisplayName = "Junk")
};

UENUM(BlueprintType)
enum class EItemClass : uint8
{
	NONE		UMETA(DisplayName = "None"),
	Warrior		UMETA(DisplayName = "Warrior"),
	ARCHER		UMETA(DisplayName = "Archer"),
	MAGE		UMETA(DisplayName = "Mage")

	/// others
};

USTRUCT(BlueprintType)
struct FCrystalSkills
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<TSoftClassPtr<UBLAction>> Skills;
};

USTRUCT(BlueprintType)
struct FCombatStatus
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECombatStatusType Status;

	/** Number of cooldowns for which the status will be active */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Turns;

	/** Percentage chance of status application. 0 - 0%, 100 - 100% */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ApplicationChance;

	FCombatStatus()
		: Status(ECombatStatusType::NONE), Turns(0), ApplicationChance(0)
	{}

	FCombatStatus(ECombatStatusType InStatus, int32 InTurns, int32 InApplicationChance)
		: Status(InStatus), Turns(InTurns), ApplicationChance(InApplicationChance)
	{}
};

USTRUCT(BlueprintType)
struct FHeroAttributes
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Experience;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ExperienceNextLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseHealth;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseMagicEnergy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Strength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Agility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Wisdom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Endurance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TotalCrystalsLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentME;

	FHeroAttributes()
		: Level(1), Experience(0), ExperienceNextLevel(50), BaseHealth(10.f)
		, BaseMagicEnergy(10.f), Strength(1.f), Agility(1.f), Wisdom(1.f), Endurance(1.f)
		, TotalCrystalsLevel(0), CurrentHP(0.f), CurrentME(0.f)
	{}
};

USTRUCT(BlueprintType)
struct FCombatCharData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABLCombatCharacter> Class;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxME;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentME;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseAttackDMG;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseDefense;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseDodge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Cooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Strength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Agility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Wisdom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Endurance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Pierce;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECombatElementType Element;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECombatElementType WeaponElement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSet<ECombatStatusType> StatusesImmunity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCombatStatus WeaponStatus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UPaperFlipbook> Sprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UPaperZDAnimInstance> AnimInstanceClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UPaperZDAnimSequence> TakeDMGAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText SpecialActionsName;

	FCombatCharData()
		: Class(nullptr), MaxHP(0.f), MaxME(0.f), CurrentHP(0.f), CurrentME(0.f)
		, BaseAttackDMG(0.f), BaseDefense(0.f), BaseDodge(0.f), Cooldown(0.f), Strength(0.f)
		, Agility(0.f), Wisdom(0.f), Endurance(0.f), Pierce(0.f), Element(ECombatElementType::NONE)
		, WeaponElement(ECombatElementType::NONE), WeaponStatus(FCombatStatus())
		, Sprite(nullptr), AnimInstanceClass(nullptr), TakeDMGAnim(nullptr)
	{}

	FCombatCharData(const FString& InName, TSubclassOf<ABLCombatCharacter> InClass, float InMaxHP
		, float InMaxME, float InCurrentHP, float InCurrentME, float InAttackDMG, float InBaseDefense
		, float InBaseDodge, float InCooldown, float InStrength, float InAgility, float InWisdom, float InEndurance
		, float InPierce, ECombatElementType InElement, ECombatElementType InWeaponElement 
		, const TSet<ECombatStatusType>& InStatusesImmunity, const FCombatStatus& InWeaponStatus
		, UPaperFlipbook* InSprite, TSubclassOf<UPaperZDAnimInstance> InAnimClass, UPaperZDAnimSequence* InTakeDMGAnim
		, const FText& InSpecialActionsName)

		:Name(InName), Class(InClass), MaxHP(InMaxHP), MaxME(InMaxME), CurrentHP(InCurrentHP), CurrentME(InCurrentME)
		, BaseAttackDMG(InAttackDMG), BaseDefense(InBaseDefense), BaseDodge(InBaseDodge), Cooldown(InCooldown)
		, Strength(InStrength), Agility(InAgility), Wisdom(InWisdom), Endurance(InEndurance), Pierce(InPierce)
		, Element(InElement), WeaponElement(InWeaponElement), StatusesImmunity(InStatusesImmunity)
		, WeaponStatus(InWeaponStatus), Sprite(InSprite), AnimInstanceClass(InAnimClass)
		, TakeDMGAnim(InTakeDMGAnim), SpecialActionsName(InSpecialActionsName)
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
		, ECrystalColor InCrystalColor = ECrystalColor::NONE, float InMECost = 0.f
		, int32 InTargetsNum = 1.f, UObject* InActionEntry = nullptr)

		: Type(InType), Flow(InFlow), Index(InIndex), CrystalColor(InCrystalColor)
		, MECost(InMECost), TargetsNum(InTargetsNum), ActionEntry(InActionEntry)
	{}
};

USTRUCT(BlueprintType)
struct FCombatActions
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSoftClassPtr<UBLAction>> AttackActions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSoftClassPtr<UBLAction>> DefendActions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<ECrystalColor, FCrystalSkills> CrystalActions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSoftClassPtr<UBLAction>> SpecialActions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSoftClassPtr<UBLCombatItem>> ItemActions;

	FCombatActions()
	{}

	FCombatActions(const TArray<TSoftClassPtr<UBLAction>>& InAttackActions, const TArray<TSoftClassPtr<UBLAction>>& InDefendActions
		, const TMap<ECrystalColor, FCrystalSkills>& InCrystalActions, const TArray<TSoftClassPtr<UBLAction>>& InSpecialActions
		, const TArray<TSoftClassPtr<UBLCombatItem>>& InItemActions)

		: AttackActions(InAttackActions), DefendActions(InDefendActions), CrystalActions(InCrystalActions)
		, SpecialActions(InSpecialActions), ItemActions(InItemActions)
	{}
};

USTRUCT(BlueprintType)
struct FPostCombatData
{
	GENERATED_BODY()

	/** Level to open after the fight. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName LevelName;

	/** Experience gained after winning fight. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Experience;

	/** Money gained after winnging fight. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Money;

	/** Optional items gained after winning fight. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSoftClassPtr<UBLItem>> Items;

	/** Optional level sequence to play after the fight. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<ULevelSequence> LevelSequence;

	/** Optional player's position if he was in Overworld before the fight. */
	UPROPERTY(BlueprintReadWrite)
	FVector PlayerPosition;

	/** A flag telling whether a player must start from PlayerPosition. */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	bool bUseNewPlayerPosition;

	FPostCombatData()
		: Experience(0), Money(0), LevelSequence(nullptr), PlayerPosition(FVector(0.f, 0.f, 0.f)), bUseNewPlayerPosition(false)
	{}

	FPostCombatData(const FName& InName, int32 InExperience, int32 InMoney, const TArray<TSoftClassPtr<UBLItem>>& InItems
		, ULevelSequence* InLevelSequence = nullptr, bool InUseNewPlayerPosition = false)

		: LevelName(InName), Experience(InExperience), Money(InMoney), Items(InItems)
		, LevelSequence(InLevelSequence), bUseNewPlayerPosition(InUseNewPlayerPosition)
	{}
};

USTRUCT(BlueprintType)
struct FCombatData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UBLEnemyDataAsset> EnemyData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UMaterialInstance> BackgroundMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USoundBase> CombatMusic;

	/** Optional */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FText> QuestDisplayTexts;

	/** Flag if player attacked first in Overworld  */
	UPROPERTY()
	bool bSneakAttack;

	UPROPERTY()
	FName EnemyTag;

	FCombatData()
		: EnemyData(nullptr), BackgroundMaterial(nullptr), CombatMusic(nullptr), bSneakAttack(false)
	{}

	FCombatData(TObjectPtr<UBLEnemyDataAsset> InEnemyData, UMaterialInstance* InBackgroundMaterial
		, USoundBase* InCombatMusic, const TArray<FText>& InQuestDisplayTexts, bool InSneakAttack = false)

		: EnemyData(InEnemyData), BackgroundMaterial(InBackgroundMaterial), CombatMusic(InCombatMusic)
		, QuestDisplayTexts(InQuestDisplayTexts), bSneakAttack(InSneakAttack)
	{}
};


USTRUCT(BlueprintType)
struct FEnemyLevelData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Strength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Agility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Wisdom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Endurance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseME;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseAttackDMG;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseDefense;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Cooldown;

	FEnemyLevelData()
		: Strength(5.f), Agility(5.f), Wisdom(5.f), Endurance(5.f), BaseHP(10.f), BaseME(10.f), BaseAttackDMG(5.f)
		, BaseDefense(10.f), Cooldown(10.f)
	{}

	FEnemyLevelData(float InStrength, float InAgility, float InWisdom, float InEndurance, float InBaseHP, float InBaseME
		, float InBaseAttackDMG, float InBaseDefens, float InCooldown)

		: Strength(InStrength), Agility(InAgility), Wisdom(InWisdom), Endurance(InEndurance), BaseHP(InBaseHP), BaseME(InBaseME)
		, BaseAttackDMG(InBaseAttackDMG), BaseDefense(InBaseDefens), Cooldown(InCooldown)
	{}
};