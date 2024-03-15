// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLHeroDataAsset.h"

FCombatCharData UBLHeroDataAsset::CalculateBaseCombatData(int32 Index)
{
	const FString Name = Heroes[Index].HeroAttributes.Name;
	TSubclassOf<ABLCombatCharacter> Class = Heroes[Index].Class;
	const float MaxHP = Heroes[Index].HeroAttributes.MaxHealth;
	const float MaxME = Heroes[Index].HeroAttributes.MaxMagicEnergy;
	const float AttackDMG = 5 + (Heroes[Index].HeroAttributes.Strength / 3); // weapon dmg + 1/3 str - TODO: weapon dmg
	const float BaseDefense = Heroes[Index].HeroAttributes.Agility / 5;
	const float Cooldown = Heroes[Index].Cooldown;
	const ECombatElementType Element = Heroes[Index].Element;
	UPaperFlipbook* Sprite = Heroes[Index].Sprite;
	TSubclassOf<UPaperZDAnimInstance> AnimInstanceClass = Heroes[Index].AnimInstanceClass;
	UPaperZDAnimSequence* TakeDMGAnim = Heroes[Index].TakeDMGAnim;
	UPaperZDAnimSequence* HealAnim = Heroes[Index].HealAnim;

	const FCombatCharData OutData = FCombatCharData(Name, Class, MaxHP, MaxME, AttackDMG, BaseDefense, Cooldown, Element, Sprite, AnimInstanceClass, TakeDMGAnim, HealAnim);

	return OutData;
}
