// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLCrystalActionWidget.h"
#include "Components/TextBlock.h"
#include "Actions/BLAction.h"
#include "UI/Entries/BLButtonEntryWidget.h"
#include "UI/Entries/BLButtonEntryData.h"
#include "Components/ListView.h"
#include "Components/Border.h"
#include "Components/WidgetSwitcher.h"

void UBLCrystalActionWidget::AddActions(const TMap<ECrystalColor, FCrystalSkills>& InCrystalActions)
{
	if (InCrystalActions.Contains(ECrystalColor::BLUE))
	{
		CrystalsDescriptions.Add(ECrystalColor::BLUE, FCrystalDescriptions());
		const TArray<TSoftClassPtr<UBLAction>>& BlueActions = InCrystalActions.Find(ECrystalColor::BLUE)->Skills;
		UBLButtonEntryData* Crystal = NewObject<UBLButtonEntryData>();
		if (Crystal)
		{
			Crystal->Init(2, FText::FromString("BLUE"), ECrystalColor::BLUE);
			CrystalsList->AddItem(Crystal);
		}
		for (int32 Index = 0; Index < BlueActions.Num(); ++Index)
		{
			UBLAction* Action = Cast<UBLAction>(BlueActions[Index].LoadSynchronous()->GetDefaultObject());
			UBLButtonEntryData* EntryItem = NewObject<UBLButtonEntryData>();
			if (Action && EntryItem)
			{
				EntryItem->Init(Index, Action->Name, ECrystalColor::BLUE);
				BlueItems.Add(EntryItem);
				if (CrystalsDescriptions.Contains(ECrystalColor::BLUE))
				{
					CrystalsDescriptions.Find(ECrystalColor::BLUE)->Descriptions.Add(Action->Description);
				}			
			}
		}		
	}

	if (InCrystalActions.Contains(ECrystalColor::RED))
	{
		CrystalsDescriptions.Add(ECrystalColor::RED, FCrystalDescriptions());
		const TArray<TSoftClassPtr<UBLAction>>& RedActions = InCrystalActions.Find(ECrystalColor::RED)->Skills;
		UBLButtonEntryData* Crystal = NewObject<UBLButtonEntryData>();
		if (Crystal)
		{
			Crystal->Init(0, FText::FromString("RED"), ECrystalColor::RED);
			CrystalsList->AddItem(Crystal);
		}
		for (int32 Index = 0; Index < RedActions.Num(); ++Index)
		{
			UBLAction* Action = Cast<UBLAction>(RedActions[Index].LoadSynchronous()->GetDefaultObject());
			UBLButtonEntryData* EntryItem = NewObject<UBLButtonEntryData>();
			if (Action && EntryItem)
			{
				EntryItem->Init(Index, Action->Name, ECrystalColor::RED);
				RedItems.Add(EntryItem);
				if (CrystalsDescriptions.Contains(ECrystalColor::RED))
				{
					CrystalsDescriptions.Find(ECrystalColor::RED)->Descriptions.Add(Action->Description);
				}
			}
		}
	}
	
	// other colors
}

void UBLCrystalActionWidget::ResetAction()
{
	Super::ResetAction();

	//
}

void UBLCrystalActionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ColorsSwitcher->SetActiveWidget(CrystalsList);
	CrystalsList->OnItemClicked().AddUObject(this, &UBLCrystalActionWidget::OnColorClicked);

}

void UBLCrystalActionWidget::OnActionClicked(UObject* Item)
{
	//TODO: FIX THIS (can't click on ActionsList after switch
	UE_LOG(LogTemp, Warning, TEXT("ggggggg"));
	ResetAction();

	UBLButtonEntryWidget* Button = Cast<UBLButtonEntryWidget>(ActionsList->GetEntryWidgetFromItem(Item));
	if (Button)
	{
		UE_LOG(LogTemp, Warning, TEXT("sdsd"));
		ClickedButton = Button;
		Button->Border->SetBrushColor(FLinearColor(0.3f, 0.3f, 0.3f, 1.f));
		if (CrystalsDescriptions.Contains(ClickedColor) && CrystalsDescriptions.Find(ClickedColor)->Descriptions.IsValidIndex(Button->Index))
		{
			DescDisplay->SetText(CrystalsDescriptions.Find(ClickedColor)->Descriptions[Button->Index]);
		}
		OnAction.ExecuteIfBound(ECombatActionType::ATTACK, Button->Index, ClickedColor);
	}
}

void UBLCrystalActionWidget::OnColorClicked(UObject* Item)
{
	//ResetColor();

	UBLButtonEntryWidget* Button = Cast<UBLButtonEntryWidget>(CrystalsList->GetEntryWidgetFromItem(Item));
	UBLButtonEntryData* ButtonData = Cast<UBLButtonEntryData>(Item);
	if (Button && ButtonData)
	{
		ClickedColorButton = Button;
		Button->Border->SetBrushColor(FLinearColor(0.3f, 0.3f, 0.3f, 1.f));
		ClickedColor = ButtonData->CrystalColor;
		
		switch (ClickedColor)
		{
			case ECrystalColor::RED:
			{
				UE_LOG(LogTemp, Warning, TEXT("red"));
				ActionsList->SetListItems(RedItems);
				break;
			}
			case ECrystalColor::BLUE:
			{
				UE_LOG(LogTemp, Warning, TEXT("blue"));
				ActionsList->SetListItems(BlueItems);
				break;
			}
			default: return;
		}

		ColorsSwitcher->SetActiveWidget(ActionsList);
	}
}
