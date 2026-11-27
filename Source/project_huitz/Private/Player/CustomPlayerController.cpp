// © 2026 Atlantic Games. All rights reserved.

#include "Player/CustomPlayerController.h"

#include "Player/PlayerHUD.h"
#include "Player/Components/PauseMenuComponent.h"

ACustomPlayerController::ACustomPlayerController(const FObjectInitializer& ObjectInitializer) {
	PauseMenuComponent = CreateDefaultSubobject<UPauseMenuComponent>(FName(TEXT("Pause Menu Component")));
}

void ACustomPlayerController::CreateDeathMessageWidget(TSubclassOf<UUserWidget> WidgetClass) {
	ClearDeathMessageWidget();
	DeathMessageWidget = CreateWidget<UUserWidget, APlayerController*>(this, WidgetClass);
	DeathMessageWidget->AddToPlayerScreen(1);
}

void ACustomPlayerController::ClearDeathMessageWidget() {
	if (IsValid(DeathMessageWidget)) {
		DeathMessageWidget->RemoveFromViewport();
		DeathMessageWidget = nullptr;
	}
}

UPlayerHUD* ACustomPlayerController::CreatePlayerHUDWidget(TSubclassOf<UPlayerHUD> WidgetClass) {
	if (IsValid(PlayerHUDWidget)) {
		PlayerHUDWidget->RemoveFromViewport();
		PlayerHUDWidget = nullptr;
	}
	
	PlayerHUDWidget = CreateWidget<UPlayerHUD, APlayerController*>(this, WidgetClass);
	PlayerHUDWidget->AddToPlayerScreen();
	
	return PlayerHUDWidget;
}
