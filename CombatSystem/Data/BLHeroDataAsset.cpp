// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLHeroDataAsset.h"

FCombatCharData UBLHeroDataAsset::CalculateBaseCombatData(int32 Index)
{
	const FString Name = Heroes[Index].HeroAttributes.Name;
	TSubclassOf<ABLCombatCharacter> Class = Heroes[Index].Class;
	const float MaxHP = Heroes[Index].HeroAttributes.BaseHealth + (5 * Heroes[Index].HeroAttributes.Endurance);
	const float MaxME = Heroes[Index].HeroAttributes.BaseMagicEnergy + (5 * Heroes[Index].HeroAttributes.Wisdom);
	const float BaseAttackDMG = Heroes[Index].HeroAttributes.Level + Heroes[Index].HeroAttributes.TotalCrystalsLevel + Heroes[Index].HeroAttributes.Strength / 3 + 5; //TODO: + Weapon dmg instead of "+ 5"
	const int32 BaseDefense = Heroes[Index].HeroAttributes.Level / 2 + Heroes[Index].HeroAttributes.Agility / 3 + 10 + 5; //TODO: + Armor + Helmet instead of "+ 10 + 5"
	const int32 BaseDodge = (Heroes[Index].HeroAttributes.Agility / 4);
	const float Cooldown = Heroes[Index].Cooldown;
	const int32 Strength = Heroes[Index].HeroAttributes.Strength;
	const int32 Agility = Heroes[Index].HeroAttributes.Agility;
	const int32 Wisdom = Heroes[Index].HeroAttributes.Wisdom;
	const int32 Endurance = Heroes[Index].HeroAttributes.Endurance;
	const int32 Pierce = Heroes[Index].HeroAttributes.Agility / 5;
	const ECombatElementType Element = Heroes[Index].Element;
	UPaperFlipbook* Sprite = Heroes[Index].Sprite;
	TSubclassOf<UPaperZDAnimInstance> AnimInstanceClass = Heroes[Index].AnimInstanceClass;
	UPaperZDAnimSequence* TakeDMGAnim = Heroes[Index].TakeDMGAnim;
	USoundBase* TakeDMGSound = Heroes[Index].TakeDMGSound;
	UPaperZDAnimSequence* HealAnim = Heroes[Index].HealAnim;
	USoundBase* HealSound = Heroes[Index].HealSound;

	const FCombatCharData OutData = FCombatCharData(Name, Class, MaxHP, MaxME, BaseAttackDMG, BaseDefense
													, BaseDodge, Cooldown, Strength, Agility, Wisdom
													, Endurance, Pierce, Element, Sprite, AnimInstanceClass 
													, TakeDMGAnim, TakeDMGSound, HealAnim, HealSound);

	return OutData;
}
