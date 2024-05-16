// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BLCombatUtilities.generated.h"

class UPaperSprite;
class UPaperZDAnimInstance;
class UPaperZDAnimSequence;
class UPaperFlipbook;
class ABLCombatCharacter;
class UBLAction;
class ULevelSequence;
class UBLEnemyDataAsset;
class UBLItem;
class UBLCombatItem; 
class UBLWeaponItem;
class UBLArmorItem;
class UBLHelmetItem;
class UPaperSprite;

/** Action types that the player can use in combat. */
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
	NONE				UMETA(DisplayName = "None"),
	DEFAULT				UMETA(DisplayName = "Default"),
	DEFAULT_MELEE		UMETA(DisplayName = "Default melee"),
	DEFAULT_RANGE		UMETA(DisplayName = "Default range"),
	MULTIPLE_MELEE		UMETA(DisplayName = "Multiple melee"),
	MULTIPLE_RANGE		UMETA(DisplayName = "Multiple range"),
	COLUMN_MELEE		UMETA(DisplayName = "Column melee"),
	BOUNCE_RANGE		UMETA(DisplayName = "Bounce range"),
	SUMMON_ALLIES	    UMETA(DisplayName = "Summon allies"),
	KILL_ALLIES			UMETA(DisplayName = "Kill allies"),
	WHOLE_TEAM_IN_PLACE UMETA(DisplayName = "Whole team in place")
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
	NONE				UMETA(DisplayName = "None"),
	BLEEDING			UMETA(DisplayName = "Bleeding"),
	POISONING			UMETA(DisplayName = "Poisoning"),
	BLOODLUST			UMETA(DisplayName = "Bloodlust"),
	STUNNED				UMETA(DisplayName = "Stunned"),
	BLINDING			UMETA(DisplayName = "Blinding"),
	SPEEDUP				UMETA(DisplayName = "Speedup"),
	INSPIRATION			UMETA(DisplayName = "Inspiration"),
	FLAMING				UMETA(DisplayName = "Flaming"),
	SHIELD				UMETA(DisplayName = "Shield"),
	INVISIBILITY		UMETA(DisplayName = "Invisibility"),
	SNEAK				UMETA(DisplayName = "Sneak attack"),
	DMG_BOOST			UMETA(DisplayName = "DMG boost"),
	ASTRAL_PROTECTION	UMETA(DisplayName = "Astral protection")
};


UENUM(BlueprintType)
enum class EItemType: uint8
{
	NONE	UMETA(DisplayName = "None"),
	WEAPON	UMETA(DisplayName = "Weapon"),
	ARMOR	UMETA(DisplayName = "Armor"),
	HELMET	UMETA(DisplayName = "Helmet"),
	USABLE	UMETA(DisplayName = "Helmet"),
	JUNK	UMETA(DisplayName = "Junk")
};

UENUM(BlueprintType)
enum class EItemClass : uint8
{
	NONE	UMETA(DisplayName = "None"),
	Warrior UMETA(DisplayName = "Warrior"),
	ARCHER	UMETA(DisplayName = "Archer"),
	MAGE	UMETA(DisplayName = "Mage")

	/// others
};


UENUM(BlueprintType)
enum class EHero : uint8
{
	NONE	UMETA(DisplayName = "None"),
	KROLTH	UMETA(DisplayName = "Krolth"),
	CARDIE	UMETA(DisplayName = "Cardie")
};

USTRUCT(BlueprintType)
struct FCrystalSkills
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<TSoftClassPtr<UBLAction>> Skills;

	FCrystalSkills()
	{}

	FCrystalSkills(const TArray<TSoftClassPtr<UBLAction>>& InSkills)

		: Skills(InSkills)
	{}
};

USTRUCT(BlueprintType)
struct FCrystal
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSoftClassPtr<UBLAction>> Skills;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UPaperFlipbook> CrystalFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UPaperSprite> CrystalSprite;

	FCrystal()
		: Level(1), CrystalFlipbook(nullptr), CrystalSprite(nullptr)
	{}

	FCrystal(int32 InLevel, const TArray<TSoftClassPtr<UBLAction>>& InSkills, UPaperFlipbook* InCrystalFlipbook, UPaperSprite* InCrystalSprite)

		: Level(InLevel), Skills(InSkills), CrystalFlipbook(InCrystalFlipbook), CrystalSprite(InCrystalSprite)
	{}
};

USTRUCT(BlueprintType)
struct FLevelUPData
{
	GENERATED_BODY()

	UPROPERTY()
	float Strength;

	UPROPERTY()
	float Agility;

	UPROPERTY()
	float Wisdom;

	UPROPERTY()
	float Endurance;

	UPROPERTY()
	float CrystalStrength;

	UPROPERTY()
	float CrystalAgility;

	UPROPERTY()
	float CrystalWisdom;

	UPROPERTY()
	float CrystalEndurance;

	UPROPERTY()
	ECrystalColor CrystalColor;

	UPROPERTY()
	bool bNewSkill;

	UPROPERTY()
	FText SkillName;

	UPROPERTY()
	TObjectPtr<UPaperSprite> CrystalSprite;

	FLevelUPData()
		: Strength(0.f), Agility(0.f), Wisdom(0.f), Endurance(0.f), CrystalStrength(0.f), CrystalAgility(0.f), CrystalWisdom(0.f)
		, CrystalEndurance(0.f), CrystalColor(ECrystalColor::NONE), bNewSkill(false), CrystalSprite(nullptr)
	{}

	FLevelUPData(float InStrength, float InAgility, float InWisdom, float InEndurance, float InCrystalStrength
		, float InCrystalAgility, float InCrystalWisdom, float InCrystalEndurance, ECrystalColor InCrystalColor
		, bool bInNewSkill, const FText& InSkillName, UPaperSprite* InCrystalSprite)

		: Strength(InStrength), Agility(InAgility), Wisdom(InWisdom), Endurance(InEndurance), CrystalStrength(InCrystalStrength)
		, CrystalAgility(InCrystalAgility), CrystalWisdom(InCrystalWisdom), CrystalEndurance(InCrystalEndurance), CrystalColor(InCrystalColor)
		, bNewSkill(bInNewSkill), SkillName(InSkillName), CrystalSprite(InCrystalSprite)
	{}
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
		: BaseHealth(10.f), BaseMagicEnergy(10.f), Strength(1.f), Agility(1.f), Wisdom(1.f), Endurance(1.f)
		, TotalCrystalsLevel(0), CurrentHP(0.f), CurrentME(0.f)
	{}

	FHeroAttributes(const FString& InName, float InBaseHealth, float InBaseMagicEnergy, float InStrength, float InAgility
		, float InWisdom, float InEndurance, float InTotalCrystalsLevel, float InCurrentHP, float InCurrentME)

		: Name(InName), BaseHealth(InBaseHealth), BaseMagicEnergy(InBaseMagicEnergy), Strength(InStrength), Agility(InAgility)
		, Wisdom(InWisdom), Endurance(InEndurance), TotalCrystalsLevel(InTotalCrystalsLevel), CurrentHP(InCurrentHP), CurrentME(InCurrentME)
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
		: Class(nullptr), MaxHP(0.f), MaxME(0.f), CurrentHP(0.f), CurrentME(0.f), BaseAttackDMG(0.f), BaseDefense(0.f)
		, BaseDodge(0.f), Cooldown(0.f), Strength(0.f), Agility(0.f), Wisdom(0.f), Endurance(0.f), Pierce(0.f)
		, Element(ECombatElementType::NONE), WeaponElement(ECombatElementType::NONE), WeaponStatus(FCombatStatus())
		, Sprite(nullptr), AnimInstanceClass(nullptr), TakeDMGAnim(nullptr)
	{}

	FCombatCharData(const FString& InName, TSubclassOf<ABLCombatCharacter> InClass, float InMaxHP, float InMaxME, float InCurrentHP
		, float InCurrentME, float InAttackDMG, float InBaseDefense, float InBaseDodge, float InCooldown, float InStrength
		, float InAgility, float InWisdom, float InEndurance, float InPierce, ECombatElementType InElement, ECombatElementType InWeaponElement
		, const TSet<ECombatStatusType>& InStatusesImmunity, const FCombatStatus& InWeaponStatus, UPaperFlipbook* InSprite
		, TSubclassOf<UPaperZDAnimInstance> InAnimClass, UPaperZDAnimSequence* InTakeDMGAnim, const FText& InSpecialActionsName)

		: Name(InName), Class(InClass), MaxHP(InMaxHP), MaxME(InMaxME), CurrentHP(InCurrentHP), CurrentME(InCurrentME), BaseAttackDMG(InAttackDMG)
		, BaseDefense(InBaseDefense), BaseDodge(InBaseDodge), Cooldown(InCooldown), Strength(InStrength), Agility(InAgility), Wisdom(InWisdom)
		, Endurance(InEndurance), Pierce(InPierce), Element(InElement), WeaponElement(InWeaponElement), StatusesImmunity(InStatusesImmunity)
		, WeaponStatus(InWeaponStatus), Sprite(InSprite), AnimInstanceClass(InAnimClass), TakeDMGAnim(InTakeDMGAnim), SpecialActionsName(InSpecialActionsName)
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

	FCombatActionData(ECombatActionType InType, ECombatActionFlow InFlow, int32 InIndex, ECrystalColor InCrystalColor = ECrystalColor::NONE
		, float InMECost = 0.f, int32 InTargetsNum = 1.f, UObject* InActionEntry = nullptr)

		: Type(InType), Flow(InFlow), Index(InIndex), CrystalColor(InCrystalColor), MECost(InMECost), TargetsNum(InTargetsNum), ActionEntry(InActionEntry)
	{}
};

USTRUCT(BlueprintType)
struct FCombatActions
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSoftClassPtr<UBLAction>> AttackActions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<UBLAction> DefendAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<ECrystalColor, FCrystalSkills> CrystalActions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSoftClassPtr<UBLAction>> SpecialActions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSoftClassPtr<UBLCombatItem>> ItemActions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<UBLAction> RunAwayAction;

	FCombatActions()
	{}

	FCombatActions(const TArray<TSoftClassPtr<UBLAction>>& InAttackActions, TSoftClassPtr<UBLAction> InDefendAction
		, const TMap<ECrystalColor, FCrystalSkills>& InCrystalActions, const TArray<TSoftClassPtr<UBLAction>>& InSpecialActions
		, const TArray<TSoftClassPtr<UBLCombatItem>>& InItemActions, TSoftClassPtr<UBLAction> InRunAwayAction)

		: AttackActions(InAttackActions), DefendAction(InDefendAction), CrystalActions(InCrystalActions)
		, SpecialActions(InSpecialActions), ItemActions(InItemActions), RunAwayAction(InRunAwayAction)
	{}
};

USTRUCT(BlueprintType)
struct FPostCombatData
{
	GENERATED_BODY()

	/** Map to open after the fight. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UWorld> PostCombatMap;

	/** Experience gained after winning fight. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Experience;

	/** Gold gained after winnging fight. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Gold;

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
		: PostCombatMap(nullptr), Experience(0), Gold(0), LevelSequence(nullptr), PlayerPosition(FVector(0.f, 0.f, 0.f)),
		  bUseNewPlayerPosition(false)
	{}

	FPostCombatData(TSoftObjectPtr<UWorld> InPostCombatMap, int32 InExperience, int32 InGold, const TArray<TSoftClassPtr<UBLItem>>& InItems
		, ULevelSequence* InLevelSequence = nullptr, bool InUseNewPlayerPosition = false)

		: PostCombatMap(InPostCombatMap), Experience(InExperience), Gold(InGold), Items(InItems)
		, LevelSequence(InLevelSequence), bUseNewPlayerPosition(InUseNewPlayerPosition)
	{}
};

USTRUCT(BlueprintType)
struct FCombatData
{
	GENERATED_BODY()

	/** Map on which the combat will take place.  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UWorld> CombatMap;

	/** Data asset with information about each enemy in team. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UBLEnemyDataAsset> EnemyData;

	/** Material used to display the combat background. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UMaterialInstance> BackgroundMaterial;

	/** Music played during the combat. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USoundBase> CombatMusic;

	/** Flag telling if Plyaer can use Run Away action (tries to escape combat). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanRunAway;

	/** Optional quests related. Temporary solution. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FText> QuestDisplayTexts;

	/** Flag telling if Player attacked first in Overworld. */
	UPROPERTY()
	bool bSneakAttack;

	/** Tag used to set a alive/dead flag for the enemy in Overworld. */
	UPROPERTY()
	FName EnemyTag;

	FCombatData()
		: CombatMap(nullptr), EnemyData(nullptr), BackgroundMaterial(nullptr), CombatMusic(nullptr), bCanRunAway(true), bSneakAttack(false)
	{}

	FCombatData(TSoftObjectPtr<UWorld> InCombatMap, TObjectPtr<UBLEnemyDataAsset> InEnemyData, UMaterialInstance* InBackgroundMaterial
		, USoundBase* InCombatMusic, bool InCanRunAway, const TArray<FText>& InQuestDisplayTexts, bool InSneakAttack)

		: CombatMap(InCombatMap), EnemyData(InEnemyData), BackgroundMaterial(InBackgroundMaterial), CombatMusic(InCombatMusic)
		, bCanRunAway(InCanRunAway), QuestDisplayTexts(InQuestDisplayTexts), bSneakAttack(InSneakAttack)
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Exp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Money;

	FEnemyLevelData()
		: Strength(5.f), Agility(5.f), Wisdom(5.f), Endurance(5.f), BaseHP(10.f), BaseME(10.f), BaseAttackDMG(5.f)
		, BaseDefense(10.f), Cooldown(10.f), Exp(5), Money(5)
	{}

	FEnemyLevelData(float InStrength, float InAgility, float InWisdom, float InEndurance, float InBaseHP, float InBaseME
		, float InBaseAttackDMG, float InBaseDefens, float InCooldown, int32 InExp, int32 InMoney)

		: Strength(InStrength), Agility(InAgility), Wisdom(InWisdom), Endurance(InEndurance), BaseHP(InBaseHP)
		, BaseME(InBaseME), BaseAttackDMG(InBaseAttackDMG), BaseDefense(InBaseDefens), Cooldown(InCooldown)
		, Exp(InExp), Money(InMoney)
	{}
};

USTRUCT(BlueprintType)
struct FHeroAssetInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FHeroAttributes HeroAttributes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABLCombatCharacter> Class;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText SpecialActionsName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Cooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECombatElementType Element;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UPaperFlipbook> Sprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UPaperZDAnimInstance> AnimInstanceClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UPaperZDAnimInstance> OutOfCombatAnimInstanceClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UPaperZDAnimSequence> TakeDMGAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCombatActions CombatActions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UBLWeaponItem> Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UBLArmorItem> Armor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UBLHelmetItem> Helmet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<UBLItem>> Items;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECrystalColor CurrentCrystal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<ECrystalColor, FCrystal> CrystalsData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UPaperSprite> RealSprite;

	FHeroAssetInfo()
		: Class(nullptr), Cooldown(5.f), Element(ECombatElementType::NONE), Sprite(nullptr), AnimInstanceClass(nullptr)
		, OutOfCombatAnimInstanceClass(nullptr), TakeDMGAnim(nullptr), Weapon(nullptr), Armor(nullptr), Helmet(nullptr)
		, CurrentCrystal(ECrystalColor::NONE), RealSprite(nullptr)
	{}

	FHeroAssetInfo(const FHeroAttributes& InHeroAttributes, TSubclassOf<ABLCombatCharacter> InClass, const FText& InSpecialActionsName
		, float InCooldown, ECombatElementType InElement, UPaperFlipbook* InSprite, TSubclassOf<UPaperZDAnimInstance> InAnimInstanceClass
		, TSubclassOf<UPaperZDAnimInstance> InOutOfCombatAnimInstanceClass, UPaperZDAnimSequence* InTakeDMGAnim, const FCombatActions& InCombatActions
		, TSubclassOf<UBLWeaponItem> InWeapon, TSubclassOf<UBLArmorItem> InArmor, TSubclassOf<UBLHelmetItem> InHelmet, const TArray<TObjectPtr<UBLItem>>& InItems
	    , ECrystalColor InCurrentCrystal, const TMap<ECrystalColor, FCrystal>& InCrystalsData, UPaperSprite* InRealSprite)

		: HeroAttributes(InHeroAttributes), Class(InClass), SpecialActionsName(InSpecialActionsName), Cooldown(InCooldown), Element(InElement)
		, Sprite(InSprite), AnimInstanceClass(InAnimInstanceClass), OutOfCombatAnimInstanceClass(InOutOfCombatAnimInstanceClass), TakeDMGAnim(InTakeDMGAnim)
		, CombatActions(InCombatActions), Weapon(InWeapon), Armor(InArmor), Helmet(InHelmet), Items(InItems), CurrentCrystal(InCurrentCrystal)
		, CrystalsData(InCrystalsData), RealSprite(InRealSprite)
	{}
};


USTRUCT(BlueprintType)
struct FHeroDataAssetData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FHeroAssetInfo> Heroes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Experience;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ExperienceNextLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSoftClassPtr<UBLItem>> Items;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Money;


	FHeroDataAssetData()
		: Level(1), Experience(0), ExperienceNextLevel(100), Money(0)
	{}

	FHeroDataAssetData(const TArray<FHeroAssetInfo>& InHeroes, int32 InLevel, int32 InExperience, int32 InExperienceNextLevel, const TArray<TSoftClassPtr<UBLItem>>& InItems, int32 InMoney)

		: Heroes(InHeroes), Level(InLevel), Experience(InExperience), ExperienceNextLevel(InExperienceNextLevel), Items(InItems), Money(InMoney)
	{}
};

USTRUCT(BlueprintType)
struct FSaveGameData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FHeroDataAssetData HeroesData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, bool> Enemies;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector PlayerLocation = FVector(0.f, 0.f, 0.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName MapName;

	FSaveGameData()
	{}

	FSaveGameData(const FHeroDataAssetData& InHeroesData, const TMap<FName, bool>& InEnemies, const FVector& InPlayerLocation, const FName& InMapName)

		: HeroesData(InHeroesData), Enemies(InEnemies), PlayerLocation(InPlayerLocation), MapName(InMapName)
	{}
};
