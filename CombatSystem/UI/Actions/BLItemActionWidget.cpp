// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLItemActionWidget.h"
#include "Components/TextBlock.h"
#include "Components/TileView.h"
#include "Components/Border.h"
#include "Actions/BLAction.h"
#include "UI/Entries/BLItemEntryWidget.h"
#include "UI/Entries/BLItemEntryData.h"
#include "Components/Image.h"
#include "BladeOfLegend/DAWID/Items/BLCombatItem.h"

void UBLItemActionWidget::AddActions(const TArray<TSoftClassPtr<UBLCombatItem>>& InItemsActions)
{
	for (int32 Index = 0; Index < 12; ++Index)
	{
		if (InItemsActions.IsValidIndex(Index))
		{
			UBLCombatItem* Item = Cast<UBLCombatItem>(InItemsActions[Index].LoadSynchronous()->GetDefaultObject());
			if (!Item) return;

			UBLAction* Action = Cast<UBLAction>(Item->Action->GetDefaultObject());
			UBLItemEntryData* EntryItem = NewObject<UBLItemEntryData>();
			if (Action && EntryItem)
			{
				EntryItem->Init(Index, Action->Name, Action->Flow, Item->Icon, true);
				EntryItem->OnDeleteFromList.BindUObject(this, &UBLItemActionWidget::DeleteItem);
				ItemsList->AddItem(EntryItem);
				Descriptions.Add(Action->Description);
			}
		}
		else
		{
			UBLItemEntryData* EntryItem = NewObject<UBLItemEntryData>();
			if (EntryItem)
			{
				EntryItem->Init(Index, FText::FromString(""), ECombatActionFlow::NONE, nullptr, false);
				ItemsList->AddItem(EntryItem);
			}
		}
	}
}

void UBLItemActionWidget::ResetAction()
{
	if (ClickedItem)
	{
		ClickedItem->Border->Background.OutlineSettings.Width = 2.f;
		ClickedItem->Border->Background.OutlineSettings.Color = FLinearColor(1.f, 1.f, 1.f, 1.f);
		ClickedItem = nullptr;
	}

	DescDisplay->SetText(FText::FromString(""));
}

void UBLItemActionWidget::OnActionClicked(UObject* Item)
{
	ResetAction();

	UBLItemEntryData* ItemEntry = Cast<UBLItemEntryData>(Item);
	UBLItemEntryWidget* ItemWidget = Cast<UBLItemEntryWidget>(ItemsList->GetEntryWidgetFromItem(Item));

	if (ItemEntry && ItemWidget)
	{
		ClickedItem = ItemWidget;
		ClickedItem->Border->Background.OutlineSettings.Width = 5.f;
		ClickedItem->Border->Background.OutlineSettings.Color = FLinearColor(0.1f, 1.f, 0.5f, 1.f);
		if (Descriptions.IsValidIndex(ItemEntry->Index))
		{
			DescDisplay->SetText(Descriptions[ItemEntry->Index]);
		}
		OnAction.ExecuteIfBound(FCombatActionData(ECombatActionType::ITEM, ItemEntry->ActionFlow, ItemEntry->Index, ECrystalColor::NONE, 0.f, 1, Item));
	}
}

void UBLItemActionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ClickedItem = nullptr;

	ItemsList->OnItemClicked().AddUObject(this, &UBLItemActionWidget::OnActionClicked);
}

void UBLItemActionWidget::DeleteItem(int32 Index)
{
	if (ItemsList->GetItemAt(Index))
	{
		UBLItemEntryWidget* ItemWidget = Cast<UBLItemEntryWidget>(ItemsList->GetEntryWidgetFromItem(ItemsList->GetItemAt(Index)));
		if (ItemWidget)
		{
			ItemWidget->Thumbnail->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.f));
			ItemWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	}
}
