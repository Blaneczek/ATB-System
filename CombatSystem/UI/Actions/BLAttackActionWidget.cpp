// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLAttackActionWidget.h"
#include "Components/TextBlock.h"
#include "Actions/BLAction.h"
#include "UI/Entries/BLActionEntryWidget.h"
#include "UI/Entries/BLActionEntryData.h"
#include "Components/ListView.h"
#include "Components/Border.h"

void UBLAttackActionWidget::AddActions(const TArray<TSoftClassPtr<UBLAction>>& InActions)
{
	for (int32 Index = 0; Index < InActions.Num(); ++Index)
	{
		UBLAction* Action = Cast<UBLAction>(InActions[Index].LoadSynchronous()->GetDefaultObject());
		UBLActionEntryData* EntryItem = NewObject<UBLActionEntryData>();
		if (Action && EntryItem)
		{
			EntryItem->Init(Index, Action->Name);
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

void UBLAttackActionWidget::OnActionClicked(UObject* Item)
{
	ResetAction();

	UBLActionEntryWidget* Button = Cast<UBLActionEntryWidget>(ActionsList->GetEntryWidgetFromItem(Item));
	if (Button)
	{
		ClickedButton = Button;
		Button->Border->SetBrushColor(FLinearColor(0.3f, 0.3f, 0.3f, 1.f));
		if (Descriptions.IsValidIndex(Button->Index))
		{
			DescDisplay->SetText(Descriptions[Button->Index]);
		}
		OnAction.ExecuteIfBound(ECombatActionType::ATTACK, Button->Index, ECrystalColor::NONE, 0.f, 1, nullptr);
	}
}

