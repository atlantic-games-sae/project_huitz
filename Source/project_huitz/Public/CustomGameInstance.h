// © 2026 Atlantic Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "UI/ConfirmationDialogWidgetBase.h"
#include "CustomGameInstance.generated.h"

class APlayerCharacter;
class USaveSlot_SaveGame;

UCLASS()
class PROJECT_HUITZ_API UCustomGameInstance : public UGameInstance {
	GENERATED_BODY()
	
public:
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
	
	UFUNCTION(BlueprintCallable, Category="Saving/Loading")
	void ClearSaveSlot(int SaveSlotIndex);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UConfirmationDialogWidgetBase> ConfirmationDialogWidgetClass;
	
	UFUNCTION(BlueprintCallable)
	void SpawnPlayerCharacters(APlayerController* MarksmanPlayerController, APlayerController* BrawlerPlayerController);
	
	void SpawnPlayerCharacters(int MarksmanPlayerControllerIndex, int BrawlerPlayerControllerIndex);
	
	APlayerCharacter* GetMarksmanCharacter() { return MarksmanCharacter_Ref; }
	APlayerCharacter* GetBrawlerCharacter() { return BrawlerCharacter_Ref; }
	
protected:
	void UpdateTimestampOfLastSave();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(MustImplement="/Script/project_huitz.MarksmanCharacter"))
	TSubclassOf<APlayerCharacter> MarksmanCharacterClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(MustImplement="/Script/project_huitz.BrawlerCharacter"))
	TSubclassOf<APlayerCharacter> BrawlerCharacterClass;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<APlayerCharacter> MarksmanCharacter_Ref;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<APlayerCharacter> BrawlerCharacter_Ref;
	
private:
	double TimestampOfLastSave;
};
