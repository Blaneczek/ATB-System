// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLActionEntryData.h"

void UBLActionEntryData::Init(int32 InIndex, const FText& InName, ECombatActionFlow InActionFlow, ECrystalColor InCrystalColor, float InMECost, int32 InTurnsCost, int32 InTargetsNum)
{
	Index = InIndex;
	Name = InName;
	CrystalColor = InCrystalColor;
	MECost = InMECost;
	TurnsCost = InTurnsCost;
	TargetsNum = InTargetsNum;
	ActionFlow = InActionFlow;

	TempName = InName;
}

void UBLActionEntryData::ChangeName(int32 TurnNum)
{
	FFormatNamedArguments Args;
	Args.Add(TEXT("Name"), TempName);
	Args.Add(TEXT("Turns"), TurnNum);
	const FText NewName = FText::Format(FText::FromString("{Name} Cd:{Turns}t"), Args);
	Name = NewName;
	OnNameChange.ExecuteIfBound(this);
}

void UBLActionEntryData::ChangeNameToDefault()
{
	Name = TempName;
	OnNameChange.ExecuteIfBound(this);
}
