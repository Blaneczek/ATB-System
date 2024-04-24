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

	virtual void NativeConstruct() override;

public:
	/** Sets Hero data for widget */
	void SetData(int32 InIndex, const FText& InName, float InMaxHP, float InCurrentHP, float InMaxME, float InCurrentME);

	/** Greys the border and resets cooldown bar when hero died */
	void GreyOutHero();

	/** Changes border color when Hero is selected */
	void HighlightHero();

	/** Changes border to default when Hero is deselected */
	void UnlightHero();

	/** Updates text widgets */
	void UpdateHP(float MaxHP, float CurrentHP);
	void UpdateME(float MaxME, float CurrentME);

	/** Controls CooldownBarWidget */
	void StartCooldownBar(float Cooldown);
	void PauseCooldownBar();
	void UnPauseCooldownBar();
	void ResetCooldownBar();

	bool IsDead() const { return bDied; };
	bool CanDoAction() const { return !bDied && bCanDoAction; };

	int32 GetIndex() const { return Index; };

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

	bool bDied = false;
	bool bCanDoAction = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor DefaultColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor HighlightColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor DeathColor;
};
