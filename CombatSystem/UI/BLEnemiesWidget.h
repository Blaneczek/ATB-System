// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BLEnemiesWidget.generated.h"

class UTileView;

/**
 * 
 */
UCLASS()
class BLADEOFLEGEND_API UBLEnemiesWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	void EnemyDied(int32 Index);

	void AddEnemy(int32 Index, const FString& EnemyName);
 
protected:
	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat", meta = (BindWidget))
	TObjectPtr<UTileView> EnemiesTileView;

};
