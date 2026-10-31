// © 2026 Atlantic Games. All rights reserved.

#include "Player/Components/PauseMenuComponent.h"

#include "HelperFunctions.h"
#include "Kismet/GameplayStatics.h"

void UPauseMenuComponent::PauseGame() {
	GetPlayerController()->SetInputMode(FInputModeUIOnly());
	GetPlayerController()->SetShowMouseCursor(true);
	
	UGameplayStatics::SetGamePaused(this, true);
	
	CreateBackgroundWidget();
	ShowBasePauseMenu();
}

void UPauseMenuComponent::ShowSettingsMenu() {
	ChangeForegroundWidget(SettingsMenuWidgetClass);
}

void UPauseMenuComponent::ShowBasePauseMenu() {
	ChangeForegroundWidget(PauseMenuWidgetClass);
}

void UPauseMenuComponent::ResumeGame() {
	GetPlayerController()->SetInputMode(FInputModeGameOnly());
	GetPlayerController()->SetShowMouseCursor(false);
	
	UGameplayStatics::SetGamePaused(this, false);
	
	RemoveForegroundWidget();
	RemoveBackgroundWidget();
}

void UPauseMenuComponent::QuitToMainMenu() {
	ResumeGame();
	UHelperFunctions::GetCustomGameInstance(this)->SaveGameToActiveSlot();
	UGameplayStatics::RemovePlayer(UGameplayStatics::GetPlayerController(this, 1), true);
	UGameplayStatics::OpenLevelBySoftObjectPtr(this, UHelperFunctions::GetCustomGameInstance(this)->MainMenuLevel);
}

void UPauseMenuComponent::ChangeForegroundWidget(TSubclassOf<UPauseMenuWidgetBase> WidgetClass) {
	RemoveForegroundWidget();
	
	ForegroundWidget_Ref = CreateWidget<UPauseMenuWidgetBase>(GetPlayerController(), WidgetClass);
	ForegroundWidget_Ref->AddToViewport(1);
}

void UPauseMenuComponent::CreateBackgroundWidget() {
	RemoveBackgroundWidget();
	
	BackgroundWidget_Ref = CreateWidget<UUserWidget>(GetPlayerController(), BackgroundWidgetClass);
	BackgroundWidget_Ref->AddToViewport();
}

void UPauseMenuComponent::RemoveForegroundWidget() {
	if (IsValid(ForegroundWidget_Ref)) {
		if (ForegroundWidget_Ref->IsInViewport()) {
			ForegroundWidget_Ref->RemoveFromParent();
		}
	}
}

void UPauseMenuComponent::RemoveBackgroundWidget() {
	if (IsValid(BackgroundWidget_Ref)) {
		if (BackgroundWidget_Ref->IsInViewport()) {
			BackgroundWidget_Ref->RemoveFromParent();
		}
	}
}

APlayerController* UPauseMenuComponent::GetPlayerController() const {
	return static_cast<APlayerController*>(GetOwner());
}
