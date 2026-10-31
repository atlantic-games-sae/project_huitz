// © 2026 Atlantic Games. All rights reserved.

#include "LevelScripts/MainMenuLevelScript.h"

#include "CustomGameInstance.h"
#include "HelperFunctions.h"
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

void AMainMenuLevelScript::ShowSaveSlotSelectionMenu(bool bIsOnline) {
	CurrentFlowType = bIsOnline ? EFlowType::Online : EFlowType::Local;
	ChangeForegroundWidget(SaveSlotSelectionWidgetClass);
}

void AMainMenuLevelScript::ShowSettingsMenu() {
	ChangeForegroundWidget(SettingsMenuWidgetClass);
}

void AMainMenuLevelScript::ShowBaseMainMenu() {
	CurrentFlowType = EFlowType::None;
	ChangeForegroundWidget(MainMenuWidgetClass);
}

void AMainMenuLevelScript::AttemptOnlineLogin() {
	GetCustomPlayerController()->OnSuccessfulLogin.AddUniqueDynamic(this, &AMainMenuLevelScript::OnSuccessfulLogin);
	GetCustomPlayerController()->OnFailedLogin.AddUniqueDynamic(this, &AMainMenuLevelScript::OnFailedLogin);
	RemoveForegroundWidget();
	GetCustomPlayerController()->Login();
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
	UHelperFunctions::GetCustomGameInstance(this)->ActiveSaveSlotIndex = SaveSlotIndex;
	
	UHelperFunctions::GetCustomGameInstance(this)->LoadGameFromActiveSlot();
	
	if (!IsValid(GetCustomPlayerController())) return;
	
	RemoveForegroundWidget();
	RemoveBackgroundWidget();
	
	GetCustomPlayerController()->SetInputMode(FInputModeGameOnly());
	GetCustomPlayerController()->SetShowMouseCursor(false);
	
	UGameplayStatics::CreatePlayer(GetCustomPlayerController());
}

void AMainMenuLevelScript::BeginOnlineLogin() {
	if (!IsValid(GetCustomPlayerController())) return;
	
	GetCustomPlayerController()->Login();
}

void AMainMenuLevelScript::BeginOnlinePlaySession(int SaveSlotIndex, FString LobbyId) {
	USaveSlot_SaveGame* SaveGame = Cast<USaveSlot_SaveGame>(UGameplayStatics::LoadGameFromSlot(UHelperFunctions::GetCustomGameInstance(this)->SaveSlotNames[SaveSlotIndex], 0));
	if (IsValid(SaveGame)) {
		UGameplayStatics::OpenLevelBySoftObjectPtr(this, SaveGame->LevelSaveData.Level);
	} else {
		UGameplayStatics::OpenLevelBySoftObjectPtr(this, UHelperFunctions::GetCustomGameInstance(this)->GameLevels[0]);
	}
	
	if (!IsValid(GetCustomPlayerController())) return;
	
	GetCustomPlayerController()->CreateLobby(LobbyId);
}

ACustomPlayerController* AMainMenuLevelScript::GetCustomPlayerController() const {
	return static_cast<ACustomPlayerController*>(GetWorld()->GetFirstPlayerController());
}

void AMainMenuLevelScript::OnSuccessfulLogin() {
	UnregisterLoginEventListeners();
	
	ChangeForegroundWidget(LobbySelectWidgetClass);
}

void AMainMenuLevelScript::OnFailedLogin() {
	UnregisterLoginEventListeners();
	
	ShowBaseMainMenu();
}

void AMainMenuLevelScript::UnregisterLoginEventListeners() {
	GetCustomPlayerController()->OnSuccessfulLogin.RemoveDynamic(this, &AMainMenuLevelScript::OnSuccessfulLogin);
	GetCustomPlayerController()->OnFailedLogin.RemoveDynamic(this, &AMainMenuLevelScript::OnFailedLogin);
}


