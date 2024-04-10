// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLMultipleDefaultMeleeAction.h"
#include "Characters/BLCombatCharacter.h"
#include "PaperZDAnimInstance.h"
#include "PaperZDAnimationComponent.h"
#include "Kismet/GameplayStatics.h"

void UBLMultipleDefaultMeleeAction::ActivateAction(ABLCombatCharacter* Owner)
{
	if (Owner)
	{
		Owner->MultipleDefaultMeleeAction();
		Owner->SetCurrentME(FMath::Clamp((Owner->GetCurrentME() - MECost), 0.f, Owner->GetMaxME()));
	}
}

void UBLMultipleDefaultMeleeAction::ExecuteAction(ABLCombatCharacter* Owner, ABLCombatCharacter* Target)
{
	if (!Owner || !Target || Target->GetCurrentHP() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("asdasd"));
		OnEndExecution.ExecuteIfBound();
		return;
	}

	if (ActionAnim && ActionSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ActionSound);
		FZDOnAnimationOverrideEndSignature EndAnimDel;
		EndAnimDel.BindLambda([this](bool bResult) { OnEndExecution.ExecuteIfBound(); });
		Owner->GetAnimationComponent()->GetAnimInstance()->PlayAnimationOverride(ActionAnim, "DefaultSlot", 1.f, 0.0f, EndAnimDel);
		ActionCalculations(Owner, Target);
	}
}
 