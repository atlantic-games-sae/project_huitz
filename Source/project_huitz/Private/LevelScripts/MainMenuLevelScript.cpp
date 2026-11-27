// © 2026 Atlantic Games. All rights reserved.

#include "LevelScripts/MainMenuLevelScript.h"

#include "CustomGameInstance.h"
#include "Libraries/HelperFunctions.h"
#include "SaveSystem.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UI/MainMenuWidgetBase.h"
#include "Player/CustomPlayerController.h"

void AMainMenuLevelScript::BeginPlay() {
	Super::BeginPlay();

	if (!IsValid(GetCustomPlayerController())) return;

	GetCustomPlayerController()->SetInputMode(FInputModeUIOnly());
	GetCustomPlayerController()->SetShowMouseCursor(true);

	BackgroundWidget_Ref = CreateWidget<UUserWidget>(GetCustomPlayerController(), BackgroundWidgetClass);
	BackgroundWidget_Ref->AddToViewport();

	ShowBaseMainMenu();
}

void AMainMenuLevelScript::ShowSaveSlotSelectionMenu() {
	ChangeForegroundWidget(SaveSlotSelectionWidgetClass);
}

void AMainMenuLevelScript::ShowSettingsMenu() {
	ChangeForegroundWidget(SettingsMenuWidgetClass);
}

void AMainMenuLevelScript::ShowBaseMainMenu() {
	ChangeForegroundWidget(MainMenuWidgetClass);
}

void AMainMenuLevelScript::ChangeForegroundWidget(TSubclassOf<UMainMenuWidgetBase> WidgetClass) {
	RemoveForegroundWidget();

	ForegroundWidget_Ref = CreateWidget<UMainMenuWidgetBase>(GetWorld()->GetFirstPlayerController(), WidgetClass);
	ForegroundWidget_Ref->AddToViewport(1);
}

void AMainMenuLevelScript::RemoveForegroundWidget() {
	if (IsValid(ForegroundWidget_Ref)) {
		if (ForegroundWidget_Ref->IsInViewport()) {
			ForegroundWidget_Ref->RemoveFromParent();
		}
	}
}

void AMainMenuLevelScript::RemoveBackgroundWidget() {
	if (IsValid(BackgroundWidget_Ref)) {
		if (BackgroundWidget_Ref->IsInViewport()) {
			BackgroundWidget_Ref->RemoveFromParent();
		}
	}
}

void AMainMenuLevelScript::BeginLocalPlaySession(int SaveSlotIndex) {
	if (!IsValid(GetCustomPlayerController())) return;

	RemoveForegroundWidget();
	RemoveBackgroundWidget();

	GetCustomPlayerController()->SetInputMode(FInputModeGameOnly());
	GetCustomPlayerController()->SetShowMouseCursor(false);

	UHelperFunctions::GetCustomGameInstance(this)->ActiveSaveSlotIndex = SaveSlotIndex;
	UHelperFunctions::GetCustomGameInstance(this)->LoadAndApplyWithFallback();
}

ACustomPlayerController* AMainMenuLevelScript::GetCustomPlayerController() const {
	return static_cast<ACustomPlayerController*>(GetWorld()->GetFirstPlayerController());
}
