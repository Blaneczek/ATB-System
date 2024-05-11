// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BladeOfLegend/AREK/Character/BLBaseChar.h"
#include "AIController.h"
#include "BLCombatUtilities.h"
#include "BLRangeProjectile.generated.h"

class USphereComponent;
class ABLCombatCharacter;
class UPaperFlipbook;
class UPaperZDAnimSequence;

DECLARE_DELEGATE(FOnReachedDestination);

UCLASS()
class BLADEOFLEGEND_API ABLRangeProjectile : public ABLBaseChar
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABLRangeProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	void SetData(UPaperFlipbook* ProjectileSprite);

	void FlyToTarget(ABLCombatCharacter* Target);

	void FlyToTargetBounce(ABLCombatCharacter* Target);

	void ReachedDestination(FAIRequestID RequestID, const FPathFollowingResult& Result);

	void ReachedBounceDestination(FAIRequestID RequestID, const FPathFollowingResult& Result);

public:
	FOnReachedDestination OnReachedDestination;

};
