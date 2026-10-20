// Fill out your copyright notice in the Description page of Project Settings.

#include "Multiplayer/MultiplayerGameMode.h"
#include "Player/PlayerCharacter.h"
#include "Player/CustomPlayerController.h"
#include "Multiplayer/MultiplayerGameState.h"
#include "Multiplayer/MultiplayerPlayerState.h"

AMultiplayerGameMode::AMultiplayerGameMode() {
	DefaultPawnClass = APlayerCharacter::StaticClass();
	PlayerControllerClass = ACustomPlayerController::StaticClass();
	GameStateClass = AMultiplayerGameState::StaticClass();
	PlayerStateClass = AMultiplayerPlayerState::StaticClass();
}

void AMultiplayerGameMode::BeginPlay() {
	Super::BeginPlay();
	
	if (ACustomPlayerController* PlayerController = Cast<ACustomPlayerController>(GetWorld()->GetFirstPlayerController())) {
		PlayerController->SetInputMode(FInputModeUIOnly());
		PlayerController->SetShowMouseCursor(true);
		
		UUserWidget* BackgroundWidget = CreateWidget<UUserWidget, APlayerController*>(PlayerController, BackgroundWidgetClass);
		BackgroundWidget->AddToViewport();
		PlayerController->MenuBackgroundWidget_Ref = BackgroundWidget;
		
		UUserWidget* MainMenuWidget = CreateWidget<UUserWidget, APlayerController*>(PlayerController, MainMenuWidgetClass);
		MainMenuWidget->AddToViewport(1);
		PlayerController->MainMenuWidget_Ref = MainMenuWidget;
	}
}
