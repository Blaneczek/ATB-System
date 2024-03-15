// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "BLRangeProjectile.h"
#include "PaperFlipbookComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "BLCombatCharacter.h"
#include "Navigation/PathFollowingComponent.h"

// Sets default values
ABLRangeProjectile::ABLRangeProjectile()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void ABLRangeProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABLRangeProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABLRangeProjectile::SetData(UPaperFlipbook* ProjectileSprite, float Damage, ECombatElementType Element)
{
	PaperFlipbook->SetFlipbook(ProjectileSprite);
	ProjectileDamage = Damage;
	ElementType = Element;
}

void ABLRangeProjectile::FlyToTarget(ABLCombatCharacter* Target)
{
	TargetCharacter = Target;
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		AIC->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &ABLRangeProjectile::ReachedAttackDestination);
		UE_LOG(LogTemp, Warning, TEXT("STARTED"));
		AIC->MoveToActor(Target, 10.f);
	}
}

void ABLRangeProjectile::ReachedAttackDestination(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		AIC->GetPathFollowingComponent()->OnRequestFinished.RemoveAll(this);
		TargetCharacter->HandleHitByAction(ProjectileDamage, ElementType);
		OnEndProjectile.ExecuteIfBound(true);
		Destroy();
	}
}

