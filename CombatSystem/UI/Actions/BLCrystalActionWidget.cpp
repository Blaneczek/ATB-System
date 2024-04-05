// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLCrystalActionWidget.h"
#include "Components/TextBlock.h"
#include "Actions/BLAction.h"
#include "UI/Entries/BLActionEntryWidget.h"
#include "UI/Entries/BLActionEntryData.h"
#include "Components/ListView.h"
#include "Components/Border.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"

void UBLCrystalActionWidget::AddActions(const TMap<ECrystalColor, FCrystalSkills>& InCrystalActions)
{
	for (auto& CrystalAction : InCrystalActions)
	{
		CrystalsDescriptions.Add(CrystalAction.Key, FCrystalDescriptions());
		const TArray<TSoftClassPtr<UBLAction>>& Actions = CrystalAction.Value.Skills;
		UBLActionEntryData* Crystal = NewObject<UBLActionEntryData>();
		if (Crystal)
		{
			Crystal->Init(0, UEnum::GetDisplayValueAsText(CrystalAction.Key), CrystalAction.Key);
			CrystalsList->AddItem(Crystal);
		}
		for (int32 Index = 0; Index < Actions.Num(); ++Index)
		{
			UBLAction* Action = Cast<UBLAction>(Actions[Index].LoadSynchronous()->GetDefaultObject());
			UBLActionEntryData* EntryItem = NewObject<UBLActionEntryData>();
			if (Action && EntryItem)
			{
				EntryItem->OnNameChange.BindUObject(this, &UBLCrystalActionWidget::ChangeName);
				EntryItem->Init(Index, Action->Name, CrystalAction.Key, Action->MECost, Action->TargetsNumber);
				switch (CrystalAction.Key)
				{
					case ECrystalColor::BLUE: 
					{
						BlueItems.Add(EntryItem);
						break;
					}
					case ECrystalColor::RED:
					{
						RedItems.Add(EntryItem);
						break;
					}
					// other colors
					default: break;					
				}
				if (CrystalsDescriptions.Contains(CrystalAction.Key))
				{
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
	}
}

void UBLCrystalActionWidget::ResetAction()
{
	ResetColorsAction();

	if (ClickedColorButton)
	{
		ClickedColorButton->Border->SetBrushColor(FLinearColor(0.5f, 0.5f, 0.5f, 1.f));
		ClickedColorButton = nullptr;
	}
	BTBack->SetVisibility(ESlateVisibility::Hidden);
	DescDisplay->SetText(FText::FromString(""));

	ColorsSwitcher->SetActiveWidget(CrystalsList);
}

void UBLCrystalActionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ColorsSwitcher->SetActiveWidget(CrystalsList);
	CrystalsList->OnItemClicked().AddUObject(this, &UBLCrystalActionWidget::OnColorClicked);

	BTBack->OnClicked.AddDynamic(this, &UBLCrystalActionWidget::OnBTBackClicked);
	BTBack->SetVisibility(ESlateVisibility::Hidden);
}

void UBLCrystalActionWidget::OnActionClicked(UObject* Item)
{
	ResetColorsAction();

	UBLActionEntryData* Action = Cast<UBLActionEntryData>(Item);
	if (Action && !Action->bCanBeUsed)
	{
		return;
	}

	UBLActionEntryWidget* Button = Cast<UBLActionEntryWidget>(ActionsList->GetEntryWidgetFromItem(Item));
	if (Button)
	{
		ClickedButton = Button;
		Button->Border->SetBrushColor(FLinearColor(0.3f, 0.3f, 0.3f, 1.f));
		if (CrystalsDescriptions.Contains(ClickedColor) && CrystalsDescriptions.Find(ClickedColor)->Descriptions.IsValidIndex(Button->Index))
		{
			DescDisplay->SetText(CrystalsDescriptions.Find(ClickedColor)->Descriptions[Button->Index]);
		}
		OnAction.ExecuteIfBound(ECombatActionType::CRYSTAL_SKILL, Button->Index, ClickedColor, Button->MECost, Button->TargetsNum, Item);
	}
}

void UBLCrystalActionWidget::OnColorClicked(UObject* Item)
{
	UBLActionEntryWidget* Button = Cast<UBLActionEntryWidget>(CrystalsList->GetEntryWidgetFromItem(Item));
	UBLActionEntryData* ButtonData = Cast<UBLActionEntryData>(Item);
	if (!Button || !ButtonData)
	{
		return;
	}

	ClickedColorButton = Button;
	Button->Border->SetBrushColor(FLinearColor(0.3f, 0.3f, 0.3f, 1.f));
	ClickedColor = ButtonData->CrystalColor;

	switch (ClickedColor)
	{
	case ECrystalColor::RED:
	{
		ActionsList->SetListItems(RedItems);
		break;
	}
	case ECrystalColor::BLUE:
	{
		ActionsList->SetListItems(BlueItems);
		break;
	}
	default: return;
	}

	ColorsSwitcher->SetActiveWidget(ActionsList);
	BTBack->SetVisibility(ESlateVisibility::Visible);
}

void UBLCrystalActionWidget::ResetColorsAction()
{
	if (ClickedButton)
	{
		ClickedButton->Border->SetBrushColor(FLinearColor(0.5f, 0.5f, 0.5f, 1.f));
		ClickedButton = nullptr;
	}
}

void UBLCrystalActionWidget::OnBTBackClicked()
{
	ResetAction();
}
