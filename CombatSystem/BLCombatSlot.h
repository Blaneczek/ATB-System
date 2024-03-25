// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BLCombatUtilities.h"
#include "BLCombatSlot.generated.h"

class ABLCombatCharacter;
class UBoxComponent;
class UStaticMeshComponent;
class UBLAction;

DECLARE_DELEGATE_OneParam(FOnSelectedSlot, ABLCombatSlot* /*Slot*/);
DECLARE_DELEGATE_TwoParams(FOnCharActionEnded, ABLCombatSlot* /*Slot*/, bool /*bIsEnemy*/);
DECLARE_DELEGATE_OneParam(FOnCharHealthUpdate, ABLCombatSlot* /*Slot*/);
DECLARE_DELEGATE_TwoParams(FOnCharDeath, ABLCombatSlot* /*Slot*/, bool /*bIsEnemy*/);
DECLARE_DELEGATE_ThreeParams(FOnEnemyAction, ABLCombatSlot* /*Slot*/, ECombatActionType /*ActionType*/, int32 /*ActionIndex*/);

UCLASS()
class BLADEOFLEGEND_API ABLCombatSlot : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABLCombatSlot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	ABLCombatCharacter* GetCharacter() const { return Character; };
	uint32 GetIndex() const { return Index; };
	bool IsEnemy() const { return bIsEnemy; };
	float GetCooldown() const;

	void SpawnCharacter(const FCombatCharData& BaseData, const TArray<TSoftClassPtr<UBLAction>>& AttackActions, const TArray<TSoftClassPtr<UBLAction>>& DefendActions, const TMap<ECrystalColor, FCrystalSkills>& CrystalActions);
	void SpawnCharacter(const FCombatCharData& BaseData, const TArray<TSoftClassPtr<UBLAction>>& AttackActions, const TArray<TSoftClassPtr<UBLAction>>& DefendActions);
	void PauseCharCooldown();
	void UnPauseCharCooldown();

	void DoAction(ECombatActionType ActionType, int32 ActionIndex, ABLCombatSlot* TargetSlot, ECrystalColor CrystalColor = ECrystalColor::NONE);

private:
	UFUNCTION()
	void EndCharCooldown();
	UFUNCTION()
	void ActionEnded();

	void StartCharCooldown();

	UFUNCTION()
	void HandleCharDeath();
	UFUNCTION()
	void UpdateCharHealth();

	/** Temporary solution for clicked and hovered effect */
	UFUNCTION()
	void OnBeginMouseOver(UPrimitiveComponent* TouchedComponent);
	UFUNCTION()
	void OnEndMouseOver(UPrimitiveComponent* TouchedComponent);
	/**/

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BL|Combat")
	TObjectPtr<UBoxComponent> Box;

	/** Temporary solution for clicked and hovered effect */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BL|Combat")
	TObjectPtr<UStaticMeshComponent> Platform;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BL|Combat")
	TObjectPtr<UMaterialInstance> DefaultMaterial;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BL|Combat")
	TObjectPtr<UMaterialInstance> ClickedMaterial;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BL|Combat")
	TObjectPtr<UMaterialInstance> HoveredMaterial;
	/**/

	UPROPERTY()
	bool bIsActive;
	UPROPERTY()
	bool bCanDoAction;
	UPROPERTY()
	bool bClicked;

	FOnSelectedSlot OnSelectedSlot;
	FOnCharActionEnded OnCharActionEnded;
	FOnCharHealthUpdate OnCharHealthUpdate;
	FOnCharDeath OnCharDeath;
	FOnEnemyAction OnEnemyAction;

private:
	UPROPERTY()
	TObjectPtr<ABLCombatCharacter> Character;
	UPROPERTY(EditInstanceOnly, Category = "BL|Combat")
	uint32 Index;
	UPROPERTY(EditInstanceOnly, Category = "BL|Combat")
	bool bIsEnemy;

	
}; 
