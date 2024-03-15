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

DECLARE_DELEGATE_OneParam(FOnEndProjectile, bool /*result*/);

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
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetData(UPaperFlipbook* ProjectileSprite, float Damage, ECombatElementType Element);
	void FlyToTarget(ABLCombatCharacter* Target);
	void ReachedAttackDestination(FAIRequestID RequestID, const FPathFollowingResult& Result);

public:
	FOnEndProjectile OnEndProjectile;

private:
	UPROPERTY()
	TObjectPtr<ABLCombatCharacter> TargetCharacter;

	float ProjectileDamage;
	ECombatElementType ElementType;
};
