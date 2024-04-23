// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLEnemyDataAsset.h"

void UBLEnemyDataAsset::SetEnemyTeamData(const TArray<FEnemyAssetInfo>& InEnemies)
{
	Enemies = InEnemies;
}

FCombatCharData UBLEnemyDataAsset::CreateEnemyData(int32 EnemyLevel, const FString& InName, TSubclassOf<ABLCombatCharacter> InClass, const FEnemyLevelData& LevelData
	, ECombatElementType EnemyElement, ECombatElementType WeaponElement, const TSet<ECombatStatusType>& StatusesImmunity, const FCombatStatus& WeaponStatus, UPaperFlipbook* Sprite
	, TSubclassOf<UPaperZDAnimInstance> AnimClass, UPaperZDAnimSequence* TakeDMGAnim)
{
	const float AttackDMG = FMath::RoundHalfFromZero(EnemyLevel + (LevelData.Strength / 3) + LevelData.BaseAttackDMG);

	const float MaxHP = LevelData.BaseHP + (5 * LevelData.Endurance);

	const float MaxME = LevelData.BaseME + (5 * LevelData.Wisdom);

	const float Dodge = FMath::RoundHalfFromZero(LevelData.Agility / 4);

	const float Pierce = FMath::RoundHalfFromZero(LevelData.Agility / 5);

	const float Defense = FMath::RoundHalfFromZero((EnemyLevel / 2) + (LevelData.Agility / 3) + LevelData.BaseDefense);

	const FText SpecialAttackName = FText(FText::FromString(""));

	return FCombatCharData(InName, InClass, MaxHP, MaxME, AttackDMG, Defense, Dodge, LevelData.Cooldown, LevelData.Strength
					, LevelData.Agility, LevelData.Wisdom, LevelData.Endurance, Pierce, EnemyElement, WeaponElement
					, StatusesImmunity, WeaponStatus, Sprite, AnimClass, TakeDMGAnim, SpecialAttackName);
}			

