// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLHeroDataAsset.h"
#include "BladeOfLegend/DAWID/Items/BLArmorItem.h"
#include "BladeOfLegend/DAWID/Items/BLWeaponItem.h"
#include "BladeOfLegend/DAWID/Items/BLHelmetItem.h"
#include "Actions/BLAction.h"

/*
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
}*/

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
		Level + 
		Heroes[Index].HeroAttributes.TotalCrystalsLevel + 
		(Strength / 3) + 
		Heroes[Index].Weapon.GetDefaultObject()->Damage;

	BaseAttackDMG = FMath::RoundHalfFromZero(BaseAttackDMG);

	float BaseDefense = 
		(Level / 2) + 
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

	bLevelUP = false;
	LevelUPData.Empty();

	Experience += InExperience;
	if (Experience >= ExperienceNextLevel)
	{
		LevelUP();
	}
}

float UBLHeroDataAsset::GetMaxHP(int32 Index) const
{
	if (!Heroes.IsValidIndex(Index))
	{
		return 0.f;
	}

	return Heroes[Index].HeroAttributes.BaseHealth + 
		(5 * Heroes[Index].HeroAttributes.Endurance) + 
		Heroes[Index].Armor.GetDefaultObject()->AdditionalHP;
}

float UBLHeroDataAsset::GetMaxME(int32 Index) const
{
	if (!Heroes.IsValidIndex(Index))
	{
		return 0.f;
	}

	return Heroes[Index].HeroAttributes.BaseMagicEnergy +
		(5 * Heroes[Index].HeroAttributes.Wisdom) +
		Heroes[Index].Armor.GetDefaultObject()->AdditionalME;
}

void UBLHeroDataAsset::LevelUP()
{
	bLevelUP = true;
	Level++;
	Experience = FMath::Clamp(Experience - ExperienceNextLevel, 0, ExperienceNextLevel);
	ExperienceNextLevel += 30;

	const float Strength = 2.f;
	const float Agility = 2.f;
	const float Wisdom = 2.f;
	const float Endurance = 2.f;

	int32 TempIndex = 0;

	for (auto& Hero : Heroes)
	{
		float CrystalStrength = 0.f;
		float CrystalAgility = 0.f;
		float CrystalWisdom = 0.f;
		float CrystalEndurance = 0.f;
		ECrystalColor CrystalColor = ECrystalColor::NONE;
		bool bNewSkill = false;
		FText SkillName;
		UPaperSprite* CrystalSprite = nullptr;

		int32 CrystalSkillIndex = 0;
		CrystalColor = Hero.CurrentCrystal;

		if (Hero.CrystalsData.Contains(CrystalColor))
		{
			const int32 CrystalLevel = ++Hero.CrystalsData.Find(CrystalColor)->Level;
			CrystalSprite = Hero.CrystalsData.Find(CrystalColor)->CrystalSprite;

			if (CrystalLevel % 5 == 0)
			{
				bNewSkill = true;
				CrystalSkillIndex = CrystalLevel / 5 - 1;
			}

			if (bNewSkill && Hero.CrystalsData.Find(CrystalColor)->Skills.IsValidIndex(CrystalSkillIndex))
			{
				if (!Hero.CombatActions.CrystalActions.Contains(CrystalColor))
				{
					Hero.CombatActions.CrystalActions.Add(CrystalColor, FCrystalSkills());
				}

				Hero.CombatActions.CrystalActions.Find(CrystalColor)->Skills.Add(Hero.CrystalsData.Find(CrystalColor)->Skills[CrystalSkillIndex]);

				if (UBLAction* Action = Cast<UBLAction>(Hero.CrystalsData.Find(CrystalColor)->Skills[CrystalSkillIndex].LoadSynchronous()->GetDefaultObject()))
				{
					SkillName = Action->Name;
				}
			}
			switch (CrystalColor)
			{
				case ECrystalColor::RED:
				{
					if (CrystalLevel % 3 == 0)
					{
						CrystalAgility = 1.f;
						CrystalWisdom = 1.f;
					}
					CrystalStrength = 3.f;					
					CrystalEndurance = 3.f;
					break;
				}
				case ECrystalColor::PURPLE:
				{
					if (CrystalLevel % 3 == 0)
					{
						CrystalAgility = 1.f;
						CrystalEndurance = 1.f;
					}
					CrystalWisdom = 3.f;
					CrystalStrength = 1.f;
					break;
				}
				case ECrystalColor::WHITE:
				{
					if (CrystalLevel % 3 == 0)
					{
						CrystalAgility = 1.f;
						CrystalStrength = 1.f;
					}
					CrystalWisdom = 3.f;
					CrystalEndurance = 2.f;
					break;
				}
				default: break;
			}
		}
 
		Hero.HeroAttributes.TotalCrystalsLevel++;
		Hero.HeroAttributes.Strength += Strength + CrystalStrength;
		Hero.HeroAttributes.Agility += Agility + CrystalAgility;
		Hero.HeroAttributes.Wisdom += Wisdom + CrystalWisdom;
		Hero.HeroAttributes.Endurance += Endurance + CrystalEndurance;
		Hero.HeroAttributes.CurrentHP = GetMaxHP(TempIndex);
		Hero.HeroAttributes.CurrentME = GetMaxME(TempIndex);
		TempIndex++;

		LevelUPData.Add(FLevelUPData(Strength, Agility, Wisdom, Endurance, CrystalStrength, CrystalAgility, CrystalWisdom, CrystalEndurance, CrystalColor, bNewSkill, SkillName, CrystalSprite));
	}
}

FHeroDataAssetData UBLHeroDataAsset::GetDataAssetAsStruct()
{
	return FHeroDataAssetData(Heroes, Level, Experience, ExperienceNextLevel, Items, Money);
}

void UBLHeroDataAsset::SetDataAssetFromStruct(const FHeroDataAssetData& Data)
{
	Heroes = Data.Heroes;
	Level = Data.Level;
	Experience = Data.Experience;
	ExperienceNextLevel = Data.ExperienceNextLevel;
	Items = Data.Items;
	Money = Data.Money;
}


