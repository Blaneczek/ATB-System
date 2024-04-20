// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLSpecialActionWidget.h"
#include "Components/TextBlock.h"
#include "Actions/BLAction.h"
#include "UI/Entries/BLActionEntryData.h"
#include "Components/ListView.h"
#include "Components/Border.h"
#include "UI/Entries/BLActionEntryWidget.h"

void UBLSpecialActionWidget::AddActions(const TArray<TSoftClassPtr<UBLAction>>& InActions)
{
	for (int32 Index = 0; Index < InActions.Num(); ++Index)
	{
		UBLAction* Action = Cast<UBLAction>(InActions[Index].LoadSynchronous()->GetDefaultObject());
		UBLActionEntryData* EntryItem = NewObject<UBLActionEntryData>();
		if (Action && EntryItem)
		{
			EntryItem->Init(Index, Action->Name, Action->Flow, ECrystalColor::NONE, Action->MECost, Action->TurnsCost, Action->TargetsNumber);
			EntryItem->OnNameChange.BindUObject(this, &UBLSpecialActionWidget::ChangeName);
			ActionsList->AddItem(EntryItem);
			FFormatNamedArguments Args;
			Args.Add(TEXT("Desc"), Action->Description);
			Args.Add(TEXT("MECost"), Action->MECost);
			Args.Add(TEXT("TurnsCost"), Action->TurnsCost);
			Args.Add(TEXT("TargetsNum"), Action->TargetsNumber);
			const FText Desc = FText::Format(FText::FromString("{Desc}\r\rME: {MECost}\rTurns cooldown: {TurnsCost}\rTargets: {TargetsNum}"), Args);
			Descriptions.Add(Desc);
		}
	}
}

void UBLSpecialActionWidget::OnActionClicked(UObject* Item)
{
	ResetAction();

	UBLActionEntryData* Action = Cast<UBLActionEntryData>(Item);
	if (Action && !Action->bCanBeUsed)
	{
		return;
	}

	UBLActionEntryWidget* Button = Cast<UBLActionEntryWidget>(ActionsList->GetEntryWidgetFromItem(Item));
	if (Button && Action)
	{
		ClickedButton = Button; 
		Button->Border->SetBrushColor(FLinearColor(0.3f, 0.3f, 0.3f, 1.f));
		if (Descriptions.IsValidIndex(Button->Index))
		{
			ShowDescription(Button->Index);
		}
		OnAction.ExecuteIfBound(FCombatActionData(ECombatActionType::SPECIAL_SKILL, Action->ActionFlow, Button->Index, ECrystalColor::NONE, Button->MECost, Button->TargetsNum, Action));
	}
}
