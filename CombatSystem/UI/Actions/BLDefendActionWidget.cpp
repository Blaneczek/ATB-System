// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLDefendActionWidget.h"
#include "Components/TextBlock.h"
#include "Actions/BLAction.h"
#include "UI/Entries/BLActionEntryData.h"
#include "Components/ListView.h"
#include "Components/Border.h"
#include "UI/Entries/BLActionEntryWidget.h"

void UBLDefendActionWidget::AddActions(const TArray<TSoftClassPtr<UBLAction>>& InActions)
{
	for (int32 Index = 0; Index < InActions.Num(); ++Index)
	{
		UBLAction* Action = Cast<UBLAction>(InActions[Index].LoadSynchronous()->GetDefaultObject());
		UBLActionEntryData* EntryItem = NewObject<UBLActionEntryData>();
		if (Action && EntryItem)
		{
			EntryItem->Init(Index, Action->Name, Action->Flow);
			ActionsList->AddItem(EntryItem);
			Descriptions.Add(Action->Description);
		}
	}
}

