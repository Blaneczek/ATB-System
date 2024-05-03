// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLLevelUPWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "PaperSprite.h"


void UBLLevelUPWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void UBLLevelUPWidget::SetData(const TArray<FLevelUPData>& LevelUpData)
{
	if (LevelUpData.IsValidIndex(0))
	{
		SetHeroData(LevelUpData[0], Krolth, KrolthCrystal, KrolthColor, KrolthSkill);
	}
	if (LevelUpData.IsValidIndex(1))
	{
		SetHeroData(LevelUpData[1], Cardie, CardieCrystal, CardieColor, CardieSkill);
	}
}

void UBLLevelUPWidget::SetHeroData(const FLevelUPData& LevelUpData, UTextBlock* Hero, UTextBlock* HeroCrystal, UImage* HeroColor, UTextBlock* HeroSkill)
{
	HeroColor->SetBrushResourceObject(LevelUpData.CrystalSprite);

	if (LevelUpData.bNewSkill)
	{
		FFormatNamedArguments HeroSkillArgs;
		HeroSkillArgs.Add(TEXT("SKILL"), LevelUpData.SkillName);
		HeroSkill->SetText(FText::Format(FText::FromString("SKILL UNLOCKED: {SKILL}"), HeroSkillArgs));
	}

	FFormatNamedArguments HeroArgs;
	HeroArgs.Add(TEXT("STR"), LevelUpData.Strength);
	HeroArgs.Add(TEXT("AGI"), LevelUpData.Agility);
	HeroArgs.Add(TEXT("WIS"), LevelUpData.Wisdom);
	HeroArgs.Add(TEXT("END"), LevelUpData.Endurance);
	const FText HeroTemp = FText::Format(FText::FromString("STR: +{STR}\rAGI: +{AGI}\rWIS: +{WIS}\rEND: +{END}"), HeroArgs);
	Hero->SetText(HeroTemp);

	switch (LevelUpData.CrystalColor)
	{
		case ECrystalColor::RED:
		{
			if (LevelUpData.CrystalAgility > 0)
			{
				FFormatNamedArguments HeroCrystalArgs;
				HeroCrystalArgs.Add(TEXT("STR"), LevelUpData.CrystalStrength);
				HeroCrystalArgs.Add(TEXT("END"), LevelUpData.CrystalEndurance);
				HeroCrystalArgs.Add(TEXT("AGI"), LevelUpData.CrystalAgility);
				HeroCrystalArgs.Add(TEXT("WIS"), LevelUpData.CrystalWisdom);
				const FText HeroCrystalTemp = FText::Format(FText::FromString("STR: +{STR}\rEND: +{END}\rAGI: +{AGI}\rWIS: +{WIS}"), HeroCrystalArgs);
				HeroCrystal->SetText(HeroCrystalTemp);
			}
			else
			{
				FFormatNamedArguments HeroCrystalArgs;
				HeroCrystalArgs.Add(TEXT("STR"), LevelUpData.CrystalStrength);
				HeroCrystalArgs.Add(TEXT("END"), LevelUpData.CrystalEndurance);
				const FText HeroCrystalTemp = FText::Format(FText::FromString("STR: +{STR}\rEND: +{END}"), HeroCrystalArgs);
				HeroCrystal->SetText(HeroCrystalTemp);
			}
			break;
		}
		case ECrystalColor::PURPLE:
		{
			if (LevelUpData.CrystalAgility > 0)
			{
				FFormatNamedArguments HeroCrystalArgs;
				HeroCrystalArgs.Add(TEXT("WIS"), LevelUpData.CrystalWisdom);
				HeroCrystalArgs.Add(TEXT("STR"), LevelUpData.CrystalStrength);
				HeroCrystalArgs.Add(TEXT("AGI"), LevelUpData.CrystalAgility);
				HeroCrystalArgs.Add(TEXT("END"), LevelUpData.CrystalEndurance);
				const FText HeroCrystalTemp = FText::Format(FText::FromString("WIS: +{WIS}\rSTR: +{STR}\rAGI: +{AGI}\rEND: +{END}"), HeroCrystalArgs);
				HeroCrystal->SetText(HeroCrystalTemp);
			}
			else
			{
				FFormatNamedArguments HeroCrystalArgs;
				HeroCrystalArgs.Add(TEXT("WIS"), LevelUpData.CrystalWisdom);
				HeroCrystalArgs.Add(TEXT("STR"), LevelUpData.CrystalStrength);
				const FText HeroCrystalTemp = FText::Format(FText::FromString("WIS: +{WIS}\rSTR: +{STR}"), HeroCrystalArgs);
				HeroCrystal->SetText(HeroCrystalTemp);
			}
			break;
		}
		case ECrystalColor::WHITE:
		{
			if (LevelUpData.CrystalAgility > 0)
			{
				FFormatNamedArguments HeroCrystalArgs;
				HeroCrystalArgs.Add(TEXT("WIS"), LevelUpData.CrystalWisdom);
				HeroCrystalArgs.Add(TEXT("END"), LevelUpData.CrystalEndurance);
				HeroCrystalArgs.Add(TEXT("AGI"), LevelUpData.CrystalAgility);
				HeroCrystalArgs.Add(TEXT("STR"), LevelUpData.CrystalStrength);
				const FText HeroCrystalTemp = FText::Format(FText::FromString("WIS: +{WIS}\rEND: +{END}\rAGI: +{AGI}\rSTR: +{STR}"), HeroCrystalArgs);
				HeroCrystal->SetText(HeroCrystalTemp);
			}
			else
			{
				FFormatNamedArguments HeroCrystalArgs;
				HeroCrystalArgs.Add(TEXT("WIS"), LevelUpData.CrystalWisdom);
				HeroCrystalArgs.Add(TEXT("END"), LevelUpData.CrystalEndurance);
				const FText HeroCrystalTemp = FText::Format(FText::FromString("WIS: +{WIS}\rEND: +{END}"), HeroCrystalArgs);
				HeroCrystal->SetText(HeroCrystalTemp);
			}
			break;
		}
		default: break;
	}
}
