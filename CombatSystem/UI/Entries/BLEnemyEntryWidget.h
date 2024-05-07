// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "BLEnemyEntryWidget.generated.h"

class UBorder;
class UTextBlock;
class UBLCooldownBarWidget;

/**
 * 
 */
UCLASS()
class BLADEOFLEGEND_API UBLEnemyEntryWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
protected:
	/** IUserObjectListEntry interface implementation */
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	
public:
	/** Greys the border when enemy died */
	void GreyOutEnemy();

	/** Controls CooldownBarWidget */
	void StartCooldownBar(float Cooldown);
	void PauseCooldownBar();
	void UnPauseCooldownBar();
	void ResetCooldownBar();

	bool IsDead() const { return bDied; };

public:
	UPROPERTY()
	int32 Index;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat", meta = (BindWidget))
	TObjectPtr<UBorder> Border;

	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat", meta = (BindWidget))
	TObjectPtr<UTextBlock> Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor DeathColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor DefaultColor;

	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat", meta = (BindWidget))
	TObjectPtr<UBLCooldownBarWidget> CooldownBar;

	bool bDied = false;
};
