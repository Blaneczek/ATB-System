// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLAttackActionWidget.h"
#include "Components/TextBlock.h"
#include "Actions/BLAction.h"
#include "UI/Entries/BLButtonEntryWidget.h"
#include "UI/Entries/BLButtonEntryData.h"
#include "Components/ListView.h"
#include "Components/Border.h"

void UBLAttackActionWidget::AddActions(const TArray<TSoftClassPtr<UBLAction>>& InAttackActions, float AttackDMG)
{
	for (int32 Index = 0; Index < InAttackActions.Num(); ++Index)
	{
		UBLAction* Action = Cast<UBLAction>(InAttackActions[Index].LoadSynchronous()->GetDefaultObject());
		UBLButtonEntryData* EntryItem = NewObject<UBLButtonEntryData>();
		if (Action && EntryItem)
		{
			EntryItem->Init(Index, Action->Name);
			ActionsList->AddItem(EntryItem);
			FFormatNamedArguments Args;
			Args.Add(TEXT("Desc"), Action->Description);
			Args.Add(TEXT("Elem"), UEnum::GetDisplayValueAsText(Action->Element));
			Args.Add(TEXT("DMG"), AttackDMG);
			const FText Desc = FText::Format(FText::FromString("{Desc}\rElement: {Elem}\rDMG: {DMG}"), Args);
 			Descriptions.Add(Desc);
		}
	}
}

void UBLAttackActionWidget::OnActionClicked(UObject* Item)
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
		OnAction.ExecuteIfBound(ECombatActionType::ATTACK, Button->Index, ECrystalColor::NONE, 0.f, 1, nullptr);
	}
}

