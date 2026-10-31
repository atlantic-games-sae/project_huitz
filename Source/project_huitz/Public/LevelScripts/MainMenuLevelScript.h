// © 2026 Atlantic Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "MainMenuLevelScript.generated.h"

class UMainMenuWidgetBase;
class UCustomGameInstance;
class ACustomPlayerController;

UENUM(BlueprintType)
enum class EFlowType : uint8 {
	None,
	Local,
	Online
};

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
	TSubclassOf<UMainMenuWidgetBase> LobbySelectWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Main Menu")
	TSubclassOf<UMainMenuWidgetBase> SaveSlotSelectionWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Main Menu")
	TSubclassOf<UMainMenuWidgetBase> SettingsMenuWidgetClass;
	
	UFUNCTION(BlueprintCallable, Category="Main Menu")
	void ShowSaveSlotSelectionMenu(bool bIsOnline);
	
	UFUNCTION(BlueprintCallable, Category="Main Menu")
	void ShowSettingsMenu();
	
	UFUNCTION(BlueprintCallable, Category="Main Menu")
	void ShowBaseMainMenu();
	
	UFUNCTION(BlueprintCallable, Category="Main Menu|Online")
	void AttemptOnlineLogin();
	
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
	
	UFUNCTION(BlueprintCallable, Category="Main Menu|Online")
	void BeginOnlineLogin();
	
	UFUNCTION(BlueprintCallable, Category="Main Menu|Online")
	void BeginOnlinePlaySession(int SaveSlotIndex, FString LobbyId);
	
	UPROPERTY(BlueprintReadOnly, Category="Main Menu")
	EFlowType CurrentFlowType;
	
private:
	ACustomPlayerController* GetCustomPlayerController() const;
	
	UFUNCTION()
	void OnSuccessfulLogin();
	
	UFUNCTION()
	void OnFailedLogin();
	
	void UnregisterLoginEventListeners();
};
