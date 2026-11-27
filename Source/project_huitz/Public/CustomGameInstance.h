// © 2026 Atlantic Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SaveSystem.h"
#include "Engine/GameInstance.h"
#include "UI/ConfirmationDialogWidgetBase.h"
#include "CustomGameInstance.generated.h"

class ACustomPlayerController;
class APlayerCharacter;
class USaveSlot_SaveGame;

UCLASS()
class PROJECT_HUITZ_API UCustomGameInstance : public UGameInstance {
	GENERATED_BODY()

public:
	UCustomGameInstance();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSoftObjectPtr<UWorld>> GameLevels;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UWorld> MainMenuLevel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FString> SaveSlotNames;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString GlobalSettings_SaveSlotName;

	UPROPERTY(BlueprintReadOnly)
	int ActiveSaveSlotIndex;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<USaveSlot_SaveGame> ActiveSaveSlot_SaveGame;

	UFUNCTION(BlueprintCallable, Category="Saving/Loading")
	void SaveGameToActiveSlot();

	UFUNCTION(BlueprintCallable, Category="Saving/Loading")
	void LoadGameFromActiveSlot();
	
	void LoadAndApplyWithFallback();
	
	void ApplyFromActiveSlot();

	UFUNCTION(BlueprintCallable, Category="Saving/Loading")
	void ClearSaveSlot(int SaveSlotIndex);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UConfirmationDialogWidgetBase> ConfirmationDialogWidgetClass;

	UFUNCTION(BlueprintCallable)
	void SpawnPlayerCharacters();
	
	UFUNCTION(BlueprintCallable)
	void RegisterCheckpointForAllPlayers(FVector CheckpointLocation, float CheckpointYaw);
	
	UFUNCTION(BlueprintCallable)
	void UpdatePlaytime();
	
	UFUNCTION()
	void PlayerDiedInCombat();
	
	void OnGameplayLevelBeginPlay();

protected:
	void StartPlaytimeTracking();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<APlayerCharacter> MarksmanCharacterClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<APlayerCharacter> BrawlerCharacterClass;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<APlayerCharacter> MarksmanCharacter_Ref;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<APlayerCharacter> BrawlerCharacter_Ref;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ACustomPlayerController> MarksmanPlayerController;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ACustomPlayerController> BrawlerPlayerController;

private:
	double PlaytimeComparisonValue;
	
	APlayerController* GetFirstAvailableController() const;
	
	void FallbackRegisterCheckpoint(FCharacterSaveData* TargetSaveData, const FVector& CheckpointLocation, float CheckpointYaw);
}; 
