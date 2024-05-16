// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLStatusesComponent.h"
#include "Characters/BLCombatCharacter.h"
#include "PaperFlipbookComponent.h"

UBLStatusesComponent::UBLStatusesComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UBLStatusesComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UBLStatusesComponent::GiveStatus(ECombatStatusType Status, int32 Turns)
{
	ABLCombatCharacter* OwnerChar = Cast<ABLCombatCharacter>(GetOwner());
	if (!OwnerChar)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBLStatusesComponent::GiveStatus | OwnerChar is nullptr"));
		return;
	}

	if (Statuses.Contains(Status))
	{
		Statuses[Status] = Turns;
		return;
	}

	Statuses.Add(Status, Turns);
	switch (Status)
	{
		case ECombatStatusType::STUNNED:
		{
			OwnerChar->CurrentCooldown += OwnerChar->BaseData.Cooldown;
			break;
		}
		case ECombatStatusType::BLINDING:
		{
			OwnerChar->CurrentCooldown += OwnerChar->BaseData.Cooldown * 0.5f;
			break;
		}
		case ECombatStatusType::SPEEDUP:
		{
			OwnerChar->CurrentCooldown -= OwnerChar->BaseData.Cooldown * 0.5f;
			break;
		}
		case ECombatStatusType::INSPIRATION:
		{
			OwnerChar->CurrentCooldown -= OwnerChar->BaseData.Cooldown * 0.3f;
			break;
		}
		case ECombatStatusType::SHIELD:
		{
			OwnerChar->bDefendIdle = true;
			OwnerChar->CurrentDefense += OwnerChar->BaseData.BaseDefense * 3.f;
			break;
		}
		case ECombatStatusType::SNEAK:
		{
			OwnerChar->CurrentCooldown = 0.1f;
			break;
		}
		case ECombatStatusType::INVISIBILITY:
		{
			OwnerChar->CurrentDodge = 50.f;
			OwnerChar->GetPaperFlipbook()->SetSpriteColor(FLinearColor(OwnerChar->GetPaperFlipbook()->GetSpriteColor().R, OwnerChar->GetPaperFlipbook()->GetSpriteColor().G, OwnerChar->GetPaperFlipbook()->GetSpriteColor().B, 0.6f));
			break;
		}
		case ECombatStatusType::DMG_BOOST:
		{
			OwnerChar->CurrentAttackDMG = OwnerChar->BaseData.BaseAttackDMG * 1.1f;
			break;
		}
		case ECombatStatusType::ASTRAL_PROTECTION:
		{
			OwnerChar->CurrentDefense += OwnerChar->BaseData.BaseDefense * 3.f;
			OwnerChar->bMagicImmunity = true;
			break;
		}
		default: break;
	}

	OwnerChar->SetStatusDisplayVisibility(Status, true);
}

void UBLStatusesComponent::RemoveStatus(ECombatStatusType Status)
{
	ABLCombatCharacter* OwnerChar = Cast<ABLCombatCharacter>(GetOwner());
	if (!OwnerChar)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBLStatusesComponent::RemoveStatus | OwnerChar is nullptr"));
		return;
	}

	if (!Statuses.Contains(Status))
	{
		return;
	}

	Statuses.Remove(Status);
	switch (Status)
	{
		case ECombatStatusType::STUNNED:
		{
			OwnerChar->CurrentCooldown -= OwnerChar->BaseData.Cooldown;
			break;
		}
		case ECombatStatusType::BLINDING:
		{
			OwnerChar->CurrentCooldown -= OwnerChar->BaseData.Cooldown * 0.5f;
			break;
		}
		case ECombatStatusType::SPEEDUP:
		{
			OwnerChar->CurrentCooldown += OwnerChar->BaseData.Cooldown * 0.7f;
			break;
		}
		case ECombatStatusType::INSPIRATION:
		{
			OwnerChar->CurrentCooldown += OwnerChar->BaseData.Cooldown * 0.3f;
			break;
		}
		case ECombatStatusType::SHIELD:
		{
			OwnerChar->bDefendIdle = false;
			OwnerChar->CurrentDefense -= OwnerChar->BaseData.BaseDefense * 3.f;
			break;
		}
		case ECombatStatusType::SNEAK:
		{
			OwnerChar->CurrentCooldown = OwnerChar->BaseData.Cooldown;
			break;
		}
		case ECombatStatusType::INVISIBILITY:
		{
			OwnerChar->CurrentDodge = OwnerChar->BaseData.BaseDodge;
			OwnerChar->GetPaperFlipbook()->SetSpriteColor(FLinearColor(OwnerChar->GetPaperFlipbook()->GetSpriteColor().R, OwnerChar->GetPaperFlipbook()->GetSpriteColor().G, OwnerChar->GetPaperFlipbook()->GetSpriteColor().B, 1.f));
			break;
		}
		case ECombatStatusType::DMG_BOOST:
		{
			OwnerChar->CurrentAttackDMG = OwnerChar->BaseData.BaseAttackDMG;
			break;
		}
		case ECombatStatusType::ASTRAL_PROTECTION:
		{
			OwnerChar->CurrentDefense -= OwnerChar->BaseData.BaseDefense * 3.f;
			OwnerChar->bMagicImmunity = false;
			break;
		}
		default: break;
	}

	OwnerChar->SetStatusDisplayVisibility(Status, false);
}

void UBLStatusesComponent::HandleStatuses()
{
	ABLCombatCharacter* OwnerChar = Cast<ABLCombatCharacter>(GetOwner());
	if (!OwnerChar)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBLStatusesComponent::HandleStatuses | OwnerChar is nullptr"));
		return;
	}

	TArray<ECombatStatusType> StatusesToDelete;

	float SimpleDMG = 0.f;

	for (auto& Status : Statuses)
	{
		switch (Status.Key)
		{
			case ECombatStatusType::BLEEDING:
			case ECombatStatusType::POISONING:
			{
				SimpleDMG += OwnerChar->BaseData.MaxHP * 0.05f;
				break;
			}
			case ECombatStatusType::FLAMING:
			{
				SimpleDMG += OwnerChar->BaseData.MaxHP * 0.1f;
				break;
			}
			default: break;
		}

		Status.Value--;

		if (Status.Value == 0)
		{
			StatusesToDelete.Add(Status.Key);
		}
	}

	OwnerChar->TakeSimpleDamage(SimpleDMG);

	// Remove statuses after action with 1 sec delay (it looks better).
	FTimerHandle RemoveDelay;
	FTimerDelegate RemoveDel;
	RemoveDel.BindWeakLambda(this, [this, StatusesToDelete]()
		{
			for (const auto& Status : StatusesToDelete)
			{
				RemoveStatus(Status);
			}
		});
	GetWorld()->GetTimerManager().SetTimer(RemoveDelay, RemoveDel, 1.f, false);
}

void UBLStatusesComponent::HandleHitStatuses(ABLCombatCharacter* Attacker, const TArray<FCombatStatus>& InStatuses, bool bMagicalAction)
{
	ABLCombatCharacter* OwnerChar = Cast<ABLCombatCharacter>(GetOwner());
	if (!OwnerChar)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBLStatusesComponent::HandleHitStatuses | OwnerChar is nullptr"));
		return;
	}

	// Temp array to add weapon and action statuses (weapon status only if its physical action)
	TArray<FCombatStatus> AllStatuses = InStatuses;
	if (!bMagicalAction)
	{
		AllStatuses.Add(Attacker->GetWeaponStatus());
	}

	for (const auto& Status : AllStatuses)
	{
		if (OwnerChar->BaseData.StatusesImmunity.Contains(Status.Status))
		{
			continue;
		}
		const int32 ApplicationChance = FMath::RandRange(1, 100);
		if (ApplicationChance <= Status.ApplicationChance)
		{
			GiveStatus(Status.Status, Status.Turns);
		}
	}
}


