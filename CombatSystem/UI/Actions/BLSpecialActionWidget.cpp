// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLSpecialActionWidget.h"
#include "Components/TextBlock.h"
#include "Actions/BLAction.h"
#include "UI/Entries/BLButtonEntryData.h"
#include "Components/ListView.h"
#include "Components/Border.h"
#include "UI/Entries/BLButtonEntryWidget.h"

void UBLSpecialActionWidget::AddActions(const TArray<TSoftClassPtr<UBLAction>>& InActions)
{
	for (int32 Index = 0; Index < InActions.Num(); ++Index)
	{
		UBLAction* Action = Cast<UBLAction>(InActions[Index].LoadSynchronous()->GetDefaultObject());
		UBLButtonEntryData* EntryItem = NewObject<UBLButtonEntryData>();
		if (Action && EntryItem)
		{
			EntryItem->Init(Index, Action->Name, ECrystalColor::NONE, Action->MECost, Action->TargetsNumber);
			ActionsList->AddItem(EntryItem);
			Descriptions.Add(Action->Description);
		}
	}
}

void UBLSpecialActionWidget::OnActionClicked(UObject* Item)
{
	ResetAction();

	UBLButtonEntryWidget* Button = Cast<UBLButtonEntryWidget>(ActionsList->GetEntryWidgetFromItem(Item));
	if (Button)
	{
		ClickedButton = Button;
		Button->Border->SetBrushColor(FLinearColor(0.3f, 0.3f, 0.3f, 1.f));
		if (Descriptions.IsValidIndex(Button->Index))
		{
			DescDisplay->SetText(Descriptions[Button->Index]);
		}
		OnAction.ExecuteIfBound(ECombatActionType::SPECIAL_SKILL, Button->Index, ECrystalColor::NONE, Button->MECost, Button->TargetsNum);
	}
}
