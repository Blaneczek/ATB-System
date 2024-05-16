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
class UWidgetComponent;
class UBLActionEntryData;

DECLARE_DELEGATE_OneParam(FOnSelectedSlot, ABLCombatSlot* /*Slot*/);
DECLARE_DELEGATE_TwoParams(FOnCharActionEnded, ABLCombatSlot* /*Slot*/, bool /*bIsEnemy*/);
DECLARE_DELEGATE(FOnCharHealthUpdate);
DECLARE_DELEGATE_TwoParams(FOnCharDeath, ABLCombatSlot* /*Slot*/, bool /*bIsEnemy*/);
DECLARE_DELEGATE_TwoParams(FOnEnemyAction, ABLCombatSlot* /*Slot*/, FCombatActionData&& /*ActionData*/);
DECLARE_DELEGATE_OneParam(FOnEscapeCombat, bool /*bSuccessful*/);
DECLARE_DELEGATE_OneParam(FOnCharDestroyed, int32 /*Index*/);
DECLARE_DELEGATE_FourParams(FOnCharSpawned, int32 /*Index*/, const FString& /*Name*/, int32 /*Level*/, float /*Cooldown*/);

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
	UFUNCTION(BlueprintCallable)
	ABLCombatCharacter* GetCharacter() const { return Character; };

	int32 GetIndex() const { return Index; };

	bool IsEnemy() const { return bIsEnemy; };

	bool IsActive() const { return bIsActive; };

	float GetCooldown() const;

	void SpawnHero(const FCombatCharData& BaseData, const FCombatActions& CombatActions, bool bSneakAttack);

	void SpawnEnemy(const FCombatCharData& BaseData, int32 Level, const TArray<TSoftClassPtr<UBLAction>>& Actions, bool bSneakAttack);

	void PauseCharCooldown();

	void UnPauseCharCooldown();

	/** Passes all the necessary data about Action to the owned character. 
	* @param bUseSlots - use or not slots as targets for action (e.g. if references to slots are needed for action).
	*/
	void DoAction(const TArray<ABLCombatSlot*>& TargetsSlots, const FCombatActionData& ActionData, AActor* CombatManager, bool bUseSlots = false);

	/** Selected Target effect */
	void SelectTarget(bool NewSelect);

	/** Selected Hero effect */
	void SelectHero(bool NewSelect);

	/** Hover slot effect */
	void HoverMouse(bool NewHover);

	void DestroyCharacter();

private:
	/** Called when the cooldown has ended. If hero, tries to select the slot. If enemy, calls OnEnemyAction delegate. */
	void CharCooldownEnded();

	/** Called when the Action has ended. */
	void ActionEnded();

	void StartCharCooldown();

	void HandleCharDeath();

	void UpdateCharHealth();

	void EscapeCombat(bool bSuccessful);

	/** Mouse hovered effect */
	UFUNCTION()
	void OnBeginMouseOver(UPrimitiveComponent* TouchedComponent);

	UFUNCTION()
	void OnEndMouseOver(UPrimitiveComponent* TouchedComponent);
	/**/

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BL|Combat")
	TObjectPtr<UBoxComponent> Box;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BL|Combat")
	TObjectPtr<UWidgetComponent> TargetPointer;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BL|Combat")
	TObjectPtr<USceneComponent> SpawnPoint;

	UPROPERTY()
	bool bCanDoAction;

	UPROPERTY()
	bool bClicked;

	FOnSelectedSlot OnSelectedSlot;
	FOnCharActionEnded OnCharActionEnded;
	FOnCharHealthUpdate OnCharHealthUpdate;
	FOnCharDeath OnCharDeath;
	FOnEnemyAction OnEnemyAction;
	FOnEscapeCombat OnEscapeCombat;
	FOnCharDestroyed OnCharDestroyed;
	FOnCharSpawned OnCharSpawned;

private:
	UPROPERTY()
	bool bIsActive;

	UPROPERTY()
	TObjectPtr<ABLCombatCharacter> Character;

	UPROPERTY(EditInstanceOnly, Category = "BL|Combat")
	int32 Index;

	UPROPERTY(EditInstanceOnly, Category = "BL|Combat")
	bool bIsEnemy;

	UPROPERTY(EditDefaultsOnly, Category = "BL|Combat")
	TSubclassOf<UUserWidget> TargetWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "BL|Combat")
	TSubclassOf<UUserWidget> HoverWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "BL|Combat")
	TSubclassOf<UUserWidget> HeroWidgetClass;

}; 
