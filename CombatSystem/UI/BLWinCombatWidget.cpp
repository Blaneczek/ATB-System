// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLWinCombatWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UBLWinCombatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BTNext->OnClicked.AddDynamic(this, &UBLWinCombatWidget::OnBTNextClicked);
}

void UBLWinCombatWidget::OnBTNextClicked()
{
	OnEndGame.ExecuteIfBound();
}

void UBLWinCombatWidget::SetData(int32 InExp, int32 InMoney)
{
	FFormatNamedArguments Args;
	Args.Add(TEXT("InExp"), InExp);
	Exp->SetText(FText::Format(FText::FromString("Exp: {InExp}"), Args));

	FFormatNamedArguments Args2;
	Args2.Add(TEXT("InMoney"), InMoney);
	Money->SetText(FText::Format(FText::FromString("Money: {InMoney}"), Args2));
}
