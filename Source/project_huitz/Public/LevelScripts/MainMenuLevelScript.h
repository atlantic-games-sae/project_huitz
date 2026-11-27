// © 2026 Atlantic Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "MainMenuLevelScript.generated.h"

class UMainMenuWidgetBase;
class UCustomGameInstance;
class ACustomPlayerController;

UCLASS()
class PROJECT_HUITZ_API AMainMenuLevelScript : public ALevelScriptActor {
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Main Menu")
	TSubclassOf<UUserWidget> BackgroundWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Main Menu")
	TSubclassOf<UMainMenuWidgetBase> MainMenuWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Main Menu")
	TSubclassOf<UMainMenuWidgetBase> SaveSlotSelectionWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Main Menu")
	TSubclassOf<UMainMenuWidgetBase> SettingsMenuWidgetClass;

	UFUNCTION(BlueprintCallable, Category="Main Menu")
	void ShowSaveSlotSelectionMenu();

	UFUNCTION(BlueprintCallable, Category="Main Menu")
	void ShowSettingsMenu();

	UFUNCTION(BlueprintCallable, Category="Main Menu")
	void ShowBaseMainMenu();

	UPROPERTY(BlueprintReadOnly, Category="Main Menu")
	TObjectPtr<UUserWidget> BackgroundWidget_Ref;

	UPROPERTY(BlueprintReadOnly, Category="Main Menu")
	TObjectPtr<UMainMenuWidgetBase> ForegroundWidget_Ref;

protected:
	void ChangeForegroundWidget(TSubclassOf<UMainMenuWidgetBase> WidgetClass);

	void RemoveForegroundWidget();

	void RemoveBackgroundWidget();

	UFUNCTION(BlueprintCallable, Category="Main Menu|Local")
	void BeginLocalPlaySession(int SaveSlotIndex);

private:
	ACustomPlayerController* GetCustomPlayerController() const;
};
