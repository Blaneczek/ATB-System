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
			Crystal->Init(0, UEnum::GetDisplayValueAsText(CrystalAction.Key), ECombatActionFlow::NONE, CrystalAction.Key);
			CrystalsList->AddItem(Crystal);
		}
		for (int32 Index = 0; Index < Actions.Num(); ++Index)
		{
			UBLAction* Action = Cast<UBLAction>(Actions[Index].LoadSynchronous()->GetDefaultObject());
			UBLActionEntryData* EntryItem = NewObject<UBLActionEntryData>();
			if (Action && EntryItem)
			{				
				EntryItem->Init(Index, Action->Name, Action->Flow, CrystalAction.Key, Action->MECost, Action->TurnsCost, Action->TargetsNumber);
				EntryItem->OnNameChange.BindUObject(this, &UBLCrystalActionWidget::ChangeName);
				switch (CrystalAction.Key)
				{
					case ECrystalColor::PURPLE: 
					{
						PurpleItems.Add(EntryItem);
						break;
					}
					case ECrystalColor::RED:
					{
						RedItems.Add(EntryItem);
						break;
					}
					case ECrystalColor::WHITE:
					{
						WhiteItems.Add(EntryItem);
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
					CrystalsDescriptions[CrystalAction.Key].Descriptions.Add(Desc);
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
		ClickedColorButton->SetIsClicked(false);
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
		Button->SetIsClicked(true);
		if (CrystalsDescriptions.Contains(ClickedColor) && CrystalsDescriptions.Find(ClickedColor)->Descriptions.IsValidIndex(Button->Index))
		{
			DescDisplay->SetText(CrystalsDescriptions.Find(ClickedColor)->Descriptions[Button->Index]);
		}	
	}

	OnAction.ExecuteIfBound(FCombatActionData(ECombatActionType::CRYSTAL_SKILL, Action->ActionFlow, Action->Index, ClickedColor, Action->MECost, Action->TargetsNum, Action));
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
	Button->SetIsClicked(true);
	ClickedColor = ButtonData->CrystalColor;

	switch (ClickedColor)
	{
	case ECrystalColor::RED:
	{
		ActionsList->SetListItems(RedItems);
		break;
	}
	case ECrystalColor::PURPLE:
	{
		ActionsList->SetListItems(PurpleItems);
		break;
	}
	case ECrystalColor::WHITE:
	{
		ActionsList->SetListItems(WhiteItems);
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
		ClickedButton->SetIsClicked(false);
		ClickedButton = nullptr;
	}
}

void UBLCrystalActionWidget::OnBTBackClicked()
{
	ResetAction();
}
