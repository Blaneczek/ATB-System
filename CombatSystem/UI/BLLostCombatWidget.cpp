// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLLostCombatWidget.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

void UBLLostCombatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BTNext->OnClicked.AddDynamic(this, &UBLLostCombatWidget::OnBTNextClicked);
	BTExit->OnClicked.AddDynamic(this, &UBLLostCombatWidget::OnBTExitClicked);
}

void UBLLostCombatWidget::OnBTNextClicked()
{
	OnLostGame.ExecuteIfBound();
	RemoveFromParent();
}

void UBLLostCombatWidget::OnBTExitClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("MAP_Menu")));
}
