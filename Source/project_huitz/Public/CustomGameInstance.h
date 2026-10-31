// © 2026 Atlantic Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "UI/ConfirmationDialogWidgetBase.h"
#include "CustomGameInstance.generated.h"

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
	
	void UpdateTimestampOfLastSave();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UConfirmationDialogWidgetBase> ConfirmationDialogWidgetClass;
	
private:
	double TimestampOfLastSave;
};
