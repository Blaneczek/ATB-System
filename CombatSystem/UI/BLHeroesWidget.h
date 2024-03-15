// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BLHeroesWidget.generated.h"

class UTileView;

/**
 * 
 */
UCLASS()
class BLADEOFLEGEND_API UBLHeroesWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	void HeroDied(int32 Index);

	void AddHero(int32 Index, const FString& HeroName, float HP, float ME);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "BL|Combat", meta = (BindWidget))
	TObjectPtr<UTileView> HeroesTileView;
};
