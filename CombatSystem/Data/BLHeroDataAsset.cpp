// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLHeroDataAsset.h"
#include "BladeOfLegend/DAWID/Items/BLArmorItem.h"
#include "BladeOfLegend/DAWID/Items/BLWeaponItem.h"
#include "BladeOfLegend/DAWID/Items/BLHelmetItem.h"

void FHeroAssetInfo::UpdateCurrentHP()
{
#pragma region NullChecks
	if (!Armor)
	{
		UE_LOG(LogTemp, Warning, TEXT("FHeroAssetInfo::UpdateStats|Armor is nullptr"))
		return;
	}
#pragma endregion NullChecks

	HeroAttributes.CurrentHP =
		HeroAttributes.BaseHealth +
		(5 * HeroAttributes.Endurance) +
		Armor.GetDefaultObject()->AdditionalHP;
}

void FHeroAssetInfo::UpdateCurrentME()
{
#pragma region NullChecks
	if (!Armor)
	{
		UE_LOG(LogTemp, Warning, TEXT("FHeroAssetInfo::UpdateCurrentME|Armor is nullptr"))
		return;
	}
#pragma endregion NullChecks

	HeroAttributes.CurrentME =
		HeroAttributes.BaseMagicEnergy +
		(5 * HeroAttributes.Wisdom) +
		Armor.GetDefaultObject()->AdditionalME;
}

void UBLHeroDataAsset::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// Get the name of the property that was changed
	if (PropertyChangedEvent.Property)
	{
		const FName PropertyName = PropertyChangedEvent.Property->GetFName();

		// Check if the changed property is one of the specified properties
		if (PropertyName == GET_MEMBER_NAME_CHECKED(FHeroAssetInfo, Armor))
		{
			for (FHeroAssetInfo& Hero : Heroes)
			{
				Hero.UpdateCurrentHP();
				Hero.UpdateCurrentME();
			}
		}
		if (PropertyName == GET_MEMBER_NAME_CHECKED(FHeroAttributes, Endurance) ||
			PropertyName == GET_MEMBER_NAME_CHECKED(FHeroAttributes, BaseHealth))
		{
			for (FHeroAssetInfo& Hero : Heroes)
			{
				Hero.UpdateCurrentHP();
			}
		}
		if (PropertyName == GET_MEMBER_NAME_CHECKED(FHeroAttributes, Wisdom) ||
			PropertyName == GET_MEMBER_NAME_CHECKED(FHeroAttributes, BaseMagicEnergy))
		{
			for (FHeroAssetInfo& Hero : Heroes)
			{
				Hero.UpdateCurrentME();
			}
		}
	}
}

FCombatCharData UBLHeroDataAsset::CalculateBaseCombatData(int32 Index)
{
	const FString Name = Heroes[Index].HeroAttributes.Name;

	TSubclassOf<ABLCombatCharacter> Class = Heroes[Index].Class;

	const float Strength = 
		Heroes[Index].HeroAttributes.Strength +
		Heroes[Index].Armor.GetDefaultObject()->Strength +
		Heroes[Index].Helmet.GetDefaultObject()->Strength;

	const float Agility = 
		Heroes[Index].HeroAttributes.Agility + 
		Heroes[Index].Armor.GetDefaultObject()->Agility +
		Heroes[Index].Helmet.GetDefaultObject()->Agility;

	const float Wisdom = 
		Heroes[Index].HeroAttributes.Wisdom +
		Heroes[Index].Armor.GetDefaultObject()->Wisdom + 
		Heroes[Index].Helmet.GetDefaultObject()->Wisdom;

	const float Endurance = 
		Heroes[Index].HeroAttributes.Endurance + 
		Heroes[Index].Armor.GetDefaultObject()->Endurance +
		Heroes[Index].Helmet.GetDefaultObject()->Endurance;

	const float MaxHP = GetMaxHP(Index);

	const float MaxME = GetMaxME(Index);

	float BaseAttackDMG = 
		Heroes[Index].HeroAttributes.Level + 
		Heroes[Index].HeroAttributes.TotalCrystalsLevel + 
		(Strength / 3) + 
		Heroes[Index].Weapon.GetDefaultObject()->Damage;

	BaseAttackDMG = FMath::RoundHalfFromZero(BaseAttackDMG);

	float BaseDefense = 
		(Heroes[Index].HeroAttributes.Level / 2) + 
		(Agility / 3) +
		Heroes[Index].Armor.GetDefaultObject()->Defense + 
		Heroes[Index].Helmet.GetDefaultObject()->Defense;

	BaseDefense = FMath::RoundHalfFromZero(BaseDefense);

	const float BaseDodge = FMath::RoundHalfFromZero(Agility / 4);

	const float CooldownTemp = 
		Heroes[Index].Cooldown - 
		Heroes[Index].Armor.GetDefaultObject()->CooldownDecrease -
		Heroes[Index].Helmet.GetDefaultObject()->CooldownDecrease - 
		Heroes[Index].Weapon.GetDefaultObject()->
		CooldownDecrease;

	const float Cooldown = FMath::Clamp(CooldownTemp, 1.f, Heroes[Index].Cooldown);

	const float Pierce = FMath::RoundHalfFromZero(Agility / 5);

	const ECombatElementType Element = Heroes[Index].Element;

	const ECombatElementType WeaponElement = Heroes[Index].Weapon.GetDefaultObject()->Element;

	TSet<ECombatStatusType> StatusesImmunity;
	StatusesImmunity.Add(Heroes[Index].Armor.GetDefaultObject()->StatusImmunity);
	StatusesImmunity.Add(Heroes[Index].Helmet.GetDefaultObject()->StatusImmunity);

	const FCombatStatus WeaponStatus = Heroes[Index].Weapon.GetDefaultObject()->Status;

	UPaperFlipbook* Sprite = Heroes[Index].Sprite;

	TSubclassOf<UPaperZDAnimInstance> AnimInstanceClass = Heroes[Index].AnimInstanceClass;

	UPaperZDAnimSequence* TakeDMGAnim = Heroes[Index].TakeDMGAnim;

	const FText SpecialActionsName = Heroes[Index].SpecialActionsName;

	return FCombatCharData(Name, Class, MaxHP, MaxME, Heroes[Index].HeroAttributes.CurrentHP,
	                       Heroes[Index].HeroAttributes.CurrentME, BaseAttackDMG, BaseDefense
	                       , BaseDodge, Cooldown, Strength, Agility, Wisdom
	                       , Endurance, Pierce, Element, WeaponElement, StatusesImmunity
	                       , WeaponStatus, Sprite, AnimInstanceClass
	                       , TakeDMGAnim, SpecialActionsName);
}

void UBLHeroDataAsset::AddCombatReward(int32 InExperience, int32 InMoney, const TArray<TSoftClassPtr<UBLItem>>& InItems)
{
	Money += InMoney;
	Items.Append(InItems);

	for (auto& Hero : Heroes)
	{
		Hero.HeroAttributes.Experience += InExperience;
	}
}

float UBLHeroDataAsset::GetMaxHP(int32 Index) const
{
	return Heroes[Index].HeroAttributes.BaseHealth + 
		(5 * Heroes[Index].HeroAttributes.Endurance) + 
		Heroes[Index].Armor.GetDefaultObject()->AdditionalHP;
}

float UBLHeroDataAsset::GetMaxME(int32 Index) const
{
	return Heroes[Index].HeroAttributes.BaseMagicEnergy + 
		(5 * Heroes[Index].HeroAttributes.Wisdom) + 
		Heroes[Index].Armor.GetDefaultObject()->AdditionalME;
}
