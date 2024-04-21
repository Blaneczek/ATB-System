// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLWinCombatWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/GridPanel.h"
#include "Components/Image.h"
#include "Blueprint/WidgetTree.h"
#include "BladeOfLegend/DAWID/Items/BLItem.h"

void UBLWinCombatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BTNext->OnClicked.AddDynamic(this, &UBLWinCombatWidget::OnBTNextClicked);
}

void UBLWinCombatWidget::OnBTNextClicked()
{
	HideAllElements();
	OnWinGame.ExecuteIfBound();
}

void UBLWinCombatWidget::SetData(int32 InExp, int32 InMoney, const TArray<TSoftClassPtr<UBLItem>>& InItems)
{
	FFormatNamedArguments Args;
	Args.Add(TEXT("InExp"), InExp);
	Exp->SetText(FText::Format(FText::FromString("Exp: {InExp}"), Args));

	FFormatNamedArguments Args2;
	Args2.Add(TEXT("InMoney"), InMoney);
	Money->SetText(FText::Format(FText::FromString("Money: {InMoney}"), Args2));

	// Show Items only if there are any items
	if (!WidgetTree)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBLWinCombatWidget::SetData | WidgetTree is invalid"));
		return;
	}

	for (int32 Index = 0; Index < InItems.Num(); ++Index)
	{
		if (!InItems[Index]) continue;

		UImage* Image = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
		UBLItem* Item = Cast<UBLItem>(InItems[Index].LoadSynchronous()->GetDefaultObject());
		if (Image && Item)
		{
			Image->SetBrushFromTexture(Item->Icon);
			Image->SetDesiredSizeOverride(FVector2D(50.f, 50.f));
			Items->AddChildToGrid(Image, 0, Index);
		}	
	}
}

void UBLWinCombatWidget::HideAllElements()
{
	Victory->SetVisibility(ESlateVisibility::Hidden);
	BTNext->SetVisibility(ESlateVisibility::Hidden);
	Exp->SetVisibility(ESlateVisibility::Hidden);
	Money->SetVisibility(ESlateVisibility::Hidden);
	Items->SetVisibility(ESlateVisibility::Hidden);
}
