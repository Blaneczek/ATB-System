// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLActionWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UBLActionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	DescDisplay->SetText(FText::FromString(""));

	BTAction->OnClicked.AddDynamic(this, &UBLActionWidget::OnBTActionClicked);
}

void UBLActionWidget::ResetAction()
{
	BTAction->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
	DescDisplay->SetText(FText::FromString(""));
}

void UBLActionWidget::OnBTActionClicked()
{
	BTAction->SetBackgroundColor(FLinearColor(0.3f, 0.3f, 0.3f, 1.f));
	DescDisplay->SetText(Description);
}
