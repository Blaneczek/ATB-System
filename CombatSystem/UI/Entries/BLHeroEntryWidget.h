// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "BLHeroEntryWidget.generated.h"

class UBorder;
class UTextBlock;
class UBLCooldownBarWidget;

/**
 * 
 */
UCLASS()
class BLADEOFLEGEND_API UBLHeroEntryWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
protected:
	/** IUserObjectListEntry interface implementation */ 
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

public:	
	void SetData(int32 InIndex, const FText& InName, float InHP, float InME);

	/** Greys the border and resets cooldown bar when hero died */
	void GreyOutHero();

	/** Updates text widgets */
	void UpdateHP(float MaxHP, float CurrentHP);
	void UpdateME(float MaxME, float CurrentME);

	/** Handle cooldown bar functions */
	void StartCooldownBar(float Cooldown);
	void PauseCooldownBar();
	void UnPauseCooldownBar();
	void ResetCooldownBar();

protected:
	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat", meta = (BindWidget))
	TObjectPtr<UBorder> Border;

	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat", meta = (BindWidget))
	TObjectPtr<UTextBlock> Name;

	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat", meta = (BindWidget))
	TObjectPtr<UTextBlock> HP;

	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat", meta = (BindWidget))
	TObjectPtr<UTextBlock> ME;

	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat", meta = (BindWidget))
	TObjectPtr<UBLCooldownBarWidget> CooldownBar;

	UPROPERTY()
	int32 Index;
};
