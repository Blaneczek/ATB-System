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
	Movement->MaxWalkSpeed = 500.f;
	PaperFlipbook->SetUsingAbsoluteRotation(false);
	TranslucencyHelperBase->SetUsingAbsoluteRotation(false);
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

void ABLRangeProjectile::SetData(UPaperFlipbook* ProjectileSprite)
{
	PaperFlipbook->SetFlipbook(ProjectileSprite);
}

void ABLRangeProjectile::FlyToTarget(ABLCombatCharacter* Target)
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC && Target)
	{	
		AIC->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &ABLRangeProjectile::ReachedDestination);
		const FVector& Location = Target->GetActorLocation() + FVector(0.f, -30.f, 0.f);
		AIC->MoveToLocation(Location, 5.f);
	}
}

void ABLRangeProjectile::FlyToTargetBounce(ABLCombatCharacter* Target)
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC && Target)
	{
		AIC->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &ABLRangeProjectile::ReachedBounceDestination);	
		const FVector& Location = Target->GetActorLocation() + FVector(0.f, -30.f, 0.f);
		AIC->MoveToLocation(Location, 5.f);
	}
}

void ABLRangeProjectile::ReachedDestination(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		AIC->GetPathFollowingComponent()->OnRequestFinished.RemoveAll(this);
		OnReachedDestination.ExecuteIfBound();
		Destroy();
	}
}

void ABLRangeProjectile::ReachedBounceDestination(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		AIC->GetPathFollowingComponent()->OnRequestFinished.RemoveAll(this);
		OnReachedDestination.ExecuteIfBound();
	}
}

