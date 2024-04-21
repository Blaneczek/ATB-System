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
	GREEN	UMETA(DisplayName = "GREEN"),
	BLUE	UMETA(DisplayName = "BLUE")
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
		: Level(1), Experience(0), ExperienceNextLevel(50), BaseHealth(10.f)
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
	float Pierce;

	UPROPERTY(EditAnywhere)
	ECombatElementType Element;

	UPROPERTY(EditAnywhere)
	ECombatElementType WeaponElement;

	UPROPERTY(EditAnywhere)
	TSet<ECombatStatusType> StatusesImmunity;

	UPROPERTY(EditAnywhere)
	FCombatStatus WeaponStatus;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UPaperFlipbook> Sprite;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPaperZDAnimInstance> AnimInstanceClass;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UPaperZDAnimSequence> TakeDMGAnim;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> TakeDMGSound;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> DeathSound;

	FCombatCharData()
		: Class(nullptr), MaxHP(0.f), MaxME(0.f), BaseAttackDMG(0.f)
		, BaseDefense(0.f), BaseDodge(0.f), Cooldown(0.f), Strength(0)
		, Agility(0), Wisdom(0), Endurance(0), Pierce(0.f), Element(ECombatElementType::NONE)
		, WeaponElement(ECombatElementType::NONE), WeaponStatus(FCombatStatus())
		, Sprite(nullptr), AnimInstanceClass(nullptr), TakeDMGAnim(nullptr)
		, TakeDMGSound(nullptr), DeathSound(nullptr)
	{}

	FCombatCharData(const FString& InName, TSubclassOf<ABLCombatCharacter> InClass, float InMaxHP
		, float InMaxME, float InAttackDMG, float InBaseDefense, float InBaseDodge
		, float InCooldown, int32 InStrength, int32 InAgility, int32 InWisdom, int32 InEndurance
		, float InPierce, ECombatElementType InElement, ECombatElementType InWeaponElement 
		, const TSet<ECombatStatusType>& InStatusesImmunity, const FCombatStatus& InWeaponStatus
		, UPaperFlipbook* InSprite, TSubclassOf<UPaperZDAnimInstance> InAnimClass, UPaperZDAnimSequence* InTakeDMGAnim
		, USoundBase* InTakeDMGSound, USoundBase* InDeathSound)

		:Name(InName), Class(InClass), MaxHP(InMaxHP), MaxME(InMaxME), BaseAttackDMG(InAttackDMG)
		, BaseDefense(InBaseDefense), BaseDodge(InBaseDodge), Cooldown(InCooldown), Strength(InStrength)
		, Agility(InAgility), Wisdom(InWisdom), Endurance(InEndurance), Pierce(InPierce), Element(InElement)
		, WeaponElement(InWeaponElement), StatusesImmunity(InStatusesImmunity), WeaponStatus(InWeaponStatus)
		, Sprite(InSprite), AnimInstanceClass(InAnimClass), TakeDMGAnim(InTakeDMGAnim)
		, TakeDMGSound(InTakeDMGSound), DeathSound(InDeathSound)
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

	UPROPERTY(EditAnywhere)
	TArray<TSoftClassPtr<UBLAction>> AttackActions;

	UPROPERTY(EditAnywhere)
	TArray<TSoftClassPtr<UBLAction>> DefendActions;

	UPROPERTY(EditAnywhere)
	TMap<ECrystalColor, FCrystalSkills> CrystalActions;

	UPROPERTY(EditAnywhere)
	TArray<TSoftClassPtr<UBLAction>> SpecialActions;

	UPROPERTY(EditAnywhere)
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

	/** Level to open after the fight */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName LevelName;

	/** Experience gained after winning fight */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Experience;

	/** Money gained after winnging fight */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Money;

	/** Optional items gained after winning fight */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSoftClassPtr<UBLItem>> Items;

	/** Optional level sequence to play after the fight */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<ULevelSequence> LevelSequence;

	/** Optional player's position if he was in Overworld before the fight */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector PlayerPosition;

	FPostCombatData()
		: Experience(0), Money(0), LevelSequence(nullptr), PlayerPosition(FVector(0.f, 0.f, 0.f))
	{}

	FPostCombatData(const FName& InName, int32 InExperience, int32 InMoney, const TArray<TSoftClassPtr<UBLItem>>& InItems
		, ULevelSequence* InLevelSequence = nullptr, const FVector& InPlayerPosition = FVector(0.f, 0.f, 0.f))

		: LevelName(InName), Experience(InExperience), Money(InMoney), Items(InItems)
		, LevelSequence(InLevelSequence), PlayerPosition(InPlayerPosition)
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

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> CombatMusic;

	/** Optional */
	UPROPERTY(EditAnywhere)
	TArray<FText> QuestDisplayTexts;

	/** Flag if player attacked first in Overworld  */
	UPROPERTY()
	bool bSneakAttack;

	FCombatData()
		: EnemyData(nullptr), BackgroundMaterial(nullptr), CombatMusic(nullptr), bSneakAttack(false)
	{}

	FCombatData(TSoftObjectPtr<UBLEnemyDataAsset> InEnemyData, UMaterialInstance* InBackgroundMaterial
		, USoundBase* InCombatMusic, const TArray<FText>& InQuestDisplayTexts, bool InSneakAttack = false)

		: EnemyData(InEnemyData), BackgroundMaterial(InBackgroundMaterial), CombatMusic(InCombatMusic)
		, QuestDisplayTexts(InQuestDisplayTexts), bSneakAttack(InSneakAttack)
	{}
};
