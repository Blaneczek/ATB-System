// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BLBackground.generated.h"

class UStaticMeshComponent;

/** Sets the appropriate combat background depending on the material assigned to a given enemy */
UCLASS()
class BLADEOFLEGEND_API ABLBackground : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABLBackground();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> Background;
};
