// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLLostCombatWidget.h"
#include "Components/Button.h"

void UBLLostCombatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BTNext->OnClicked.AddDynamic(this, &UBLLostCombatWidget::OnBTNextClicked);
}

void UBLLostCombatWidget::OnBTNextClicked()
{
	OnLostGame.ExecuteIfBound();
}
