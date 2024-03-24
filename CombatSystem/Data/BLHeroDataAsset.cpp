// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLHeroDataAsset.h"

FCombatCharData UBLHeroDataAsset::CalculateBaseCombatData(int32 Index)
{
	const FString Name = Heroes[Index].HeroAttributes.Name;
	TSubclassOf<ABLCombatCharacter> Class = Heroes[Index].Class;
	const float MaxHP = Heroes[Index].HeroAttributes.BaseHealth + (5 * Heroes[Index].HeroAttributes.Endurance);
	const float MaxME = Heroes[Index].HeroAttributes.BaseMagicEnergy + (5 * Heroes[Index].HeroAttributes.Wisdom);
	const float AttackDMG = (Heroes[Index].HeroAttributes.Strength / 3) + 5 ; // weapon dmg + 1/3 str - TODO: weapon dmg
	const float BaseDefense = (Heroes[Index].HeroAttributes.Agility / 5) + 5; // items defense + 1/5 agility - TODO: items defense
	const float BaseDodge = (Heroes[Index].HeroAttributes.Agility / 4);
	const float Cooldown = Heroes[Index].Cooldown;
	const int32 Strength = Heroes[Index].HeroAttributes.Strength;
	const int32 Agility = Heroes[Index].HeroAttributes.Agility;
	const int32 Wisdom = Heroes[Index].HeroAttributes.Wisdom;
	const int32 Endurance = Heroes[Index].HeroAttributes.Endurance;
	const ECombatElementType Element = Heroes[Index].Element;
	UPaperFlipbook* Sprite = Heroes[Index].Sprite;
	TSubclassOf<UPaperZDAnimInstance> AnimInstanceClass = Heroes[Index].AnimInstanceClass;
	UPaperZDAnimSequence* TakeDMGAnim = Heroes[Index].TakeDMGAnim;
	UPaperZDAnimSequence* HealAnim = Heroes[Index].HealAnim;

	const FCombatCharData OutData = FCombatCharData(Name, Class, MaxHP, MaxME, AttackDMG, BaseDefense
													, BaseDodge, Cooldown, Strength, Agility, Wisdom
													, Endurance, Element, Sprite, AnimInstanceClass 
													, TakeDMGAnim, HealAnim);

	return OutData;
}
