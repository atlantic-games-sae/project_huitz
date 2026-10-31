// © 2026 Atlantic Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UI/PauseMenuWidgetBase.h"
#include "PauseMenuComponent.generated.h"

class UCustomGameInstance;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT_HUITZ_API UPauseMenuComponent : public UActorComponent {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Pause Menu")
	TSubclassOf<UUserWidget> BackgroundWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Pause Menu")
	TSubclassOf<UPauseMenuWidgetBase> PauseMenuWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Pause Menu")
	TSubclassOf<UPauseMenuWidgetBase> SettingsMenuWidgetClass;
	
	UFUNCTION(BlueprintCallable, Category="Pause Menu")
	void PauseGame();
	
	UFUNCTION(BlueprintCallable, Category="Pause Menu")
	void ShowSettingsMenu();
	
	UFUNCTION(BlueprintCallable, Category="Pause Menu")
	void ShowBasePauseMenu();
	
	UFUNCTION(BlueprintCallable, Category="Pause Menu")
	void ResumeGame();
	
	UFUNCTION(BlueprintCallable, Category="Pause Menu")
	void QuitToMainMenu();
	
protected:
	UPROPERTY(BlueprintReadOnly, Category="Pause Menu")
	TObjectPtr<UUserWidget> BackgroundWidget_Ref;
	
	UPROPERTY(BlueprintReadOnly, Category="Pause Menu")
	TObjectPtr<UPauseMenuWidgetBase> ForegroundWidget_Ref;
	
	void ChangeForegroundWidget(TSubclassOf<UPauseMenuWidgetBase> WidgetClass);
	
	void CreateBackgroundWidget();
	
	void RemoveForegroundWidget();
	
	void RemoveBackgroundWidget();
	
	APlayerController* GetPlayerController() const;
};
