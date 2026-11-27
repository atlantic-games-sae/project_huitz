// © 2026 Atlantic Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "CustomPlayerController.generated.h"

class UPlayerHUD;
class UPauseMenuComponent;
class AMainMenuLevelScript;

UCLASS()
class PROJECT_HUITZ_API ACustomPlayerController : public APlayerController {
	GENERATED_BODY()

public:
	ACustomPlayerController(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Category="Components", EditDefaultsOnly, BlueprintReadOnly)
	UPauseMenuComponent* PauseMenuComponent;
	
	UFUNCTION(Category="UI", BlueprintCallable)
	void CreateDeathMessageWidget(TSubclassOf<UUserWidget> WidgetClass);
	
	UFUNCTION(Category="UI", BlueprintCallable)
	void ClearDeathMessageWidget();
	
	UFUNCTION(Category="UI", BlueprintCallable)
	UPlayerHUD* CreatePlayerHUDWidget(TSubclassOf<UPlayerHUD> WidgetClass);
	
protected:
	UPROPERTY(Category="UI", BlueprintReadOnly)
	TObjectPtr<UUserWidget> DeathMessageWidget;
	
	UPROPERTY(Category="UI", BlueprintReadOnly)
	TObjectPtr<UPlayerHUD> PlayerHUDWidget;
};
