// © 2026 Atlantic Games. All rights reserved.

#include "CustomGameInstance.h"
#include "SaveSystem.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "LevelScripts/GameplayLevelScript.h"
#include "Libraries/HelperFunctions.h"
#include "Player/CustomPlayerController.h"
#include "Player/PlayerCharacter.h"
#include "Player/Components/AttackComponent/BaseAttackComponent.h"

UCustomGameInstance::UCustomGameInstance() {
	SaveSlotNames = { "SAVE_SaveSlot1", "SAVE_SaveSlot2", "SAVE_SaveSlot3", "SAVE_SaveSlot4" };
	GlobalSettings_SaveSlotName = "SAVE_GlobalSettings";
	ActiveSaveSlotIndex = 0;
	
	PlaytimeComparisonValue = 0;
}

void UCustomGameInstance::SaveGameToActiveSlot() {
	if (!IsValid(ActiveSaveSlot_SaveGame)) {
		ActiveSaveSlot_SaveGame = NewObject<USaveSlot_SaveGame>();
	}

	ActiveSaveSlot_SaveGame->LevelSaveData.CollectedLevelPickups = {};
	ActiveSaveSlot_SaveGame->LevelSaveData.ExtraPickups = {};
	
	UpdatePlaytime();

	// Brawler and marksman save data should be gathered and added here
	if (IsValid(MarksmanCharacter_Ref)) {
		ActiveSaveSlot_SaveGame->MarksmanSaveData.Transform = MarksmanCharacter_Ref->GetLastCheckpointTransform();
		ActiveSaveSlot_SaveGame->MarksmanSaveData.CurrentHealth = MarksmanCharacter_Ref->GetCurrentHealth();
		ActiveSaveSlot_SaveGame->MarksmanSaveData.UnlockedWeapons = MarksmanCharacter_Ref->MyAttackComponent->GetOwnedWeapons();
	}

	if (IsValid(BrawlerCharacter_Ref)) {
		ActiveSaveSlot_SaveGame->BrawlerSaveData.Transform = BrawlerCharacter_Ref->GetLastCheckpointTransform();
		ActiveSaveSlot_SaveGame->BrawlerSaveData.CurrentHealth = BrawlerCharacter_Ref->GetCurrentHealth();
		ActiveSaveSlot_SaveGame->BrawlerSaveData.UnlockedWeapons = BrawlerCharacter_Ref->MyAttackComponent->GetOwnedWeapons();
	}

	UGameplayStatics::SaveGameToSlot(ActiveSaveSlot_SaveGame, SaveSlotNames[ActiveSaveSlotIndex], 0);
}

void UCustomGameInstance::LoadAndApplyWithFallback() {
	LoadGameFromActiveSlot();
	
	if (!IsValid(ActiveSaveSlot_SaveGame)) {
		ActiveSaveSlot_SaveGame = NewObject<USaveSlot_SaveGame>();
	}
	
	ApplyFromActiveSlot();
}

void UCustomGameInstance::LoadGameFromActiveSlot() {
	ActiveSaveSlot_SaveGame = Cast<USaveSlot_SaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotNames[ActiveSaveSlotIndex], 0));
}

void UCustomGameInstance::ApplyFromActiveSlot() {
	if (!IsValid(ActiveSaveSlot_SaveGame)) return;
	
	UGameplayStatics::OpenLevelBySoftObjectPtr(this, GameLevels[ActiveSaveSlot_SaveGame->LevelSaveData.LevelIndex]);

	AGameplayLevelScript* GameplayLevelScript = Cast<AGameplayLevelScript>(GetWorld()->GetCurrentLevel()->GetLevelScriptActor());
	if (!IsValid(GameplayLevelScript)) return;
	GameplayLevelScript->LoadFromLevelSaveData(ActiveSaveSlot_SaveGame->LevelSaveData);
	
	if (IsValid(MarksmanCharacter_Ref)) MarksmanCharacter_Ref->LoadPlayerSaveData(ActiveSaveSlot_SaveGame->MarksmanSaveData);
	if (IsValid(BrawlerCharacter_Ref)) BrawlerCharacter_Ref->LoadPlayerSaveData(ActiveSaveSlot_SaveGame->BrawlerSaveData);
}

void UCustomGameInstance::ClearSaveSlot(int SaveSlotIndex) {
	UGameplayStatics::DeleteGameInSlot(SaveSlotNames[SaveSlotIndex], 0);
}

void UCustomGameInstance::SpawnPlayerCharacters() {
	if (!IsValid(MarksmanPlayerController)) MarksmanPlayerController = Cast<ACustomPlayerController>(GetFirstAvailableController());
	if (!IsValid(BrawlerPlayerController)) BrawlerPlayerController = Cast<ACustomPlayerController>(GetFirstAvailableController());

	if (!IsValid(MarksmanCharacter_Ref)) {
		MarksmanCharacter_Ref = Cast<APlayerCharacter>(GetWorld()->SpawnActor(MarksmanCharacterClass));
		MarksmanCharacter_Ref->LoadPlayerSaveData(ActiveSaveSlot_SaveGame->MarksmanSaveData);
	}

	if (!IsValid(BrawlerCharacter_Ref)) {
		BrawlerCharacter_Ref = Cast<APlayerCharacter>(GetWorld()->SpawnActor(BrawlerCharacterClass));
		BrawlerCharacter_Ref->LoadPlayerSaveData(ActiveSaveSlot_SaveGame->BrawlerSaveData);
	}

	MarksmanPlayerController->Possess(MarksmanCharacter_Ref);
	BrawlerPlayerController->Possess(BrawlerCharacter_Ref);
}

void UCustomGameInstance::RegisterCheckpointForAllPlayers(FVector CheckpointLocation, float CheckpointYaw) {
	if (IsValid(MarksmanCharacter_Ref)) MarksmanCharacter_Ref->RegisterCheckpoint(CheckpointLocation, CheckpointYaw);
	else FallbackRegisterCheckpoint(&ActiveSaveSlot_SaveGame->MarksmanSaveData, CheckpointLocation, CheckpointYaw);
	if (IsValid(BrawlerCharacter_Ref)) BrawlerCharacter_Ref->RegisterCheckpoint(CheckpointLocation, CheckpointYaw);
	else FallbackRegisterCheckpoint(&ActiveSaveSlot_SaveGame->BrawlerSaveData, CheckpointLocation, CheckpointYaw);
}

void UCustomGameInstance::StartPlaytimeTracking() {
	PlaytimeComparisonValue = UGameplayStatics::GetTimeSeconds(this);
}

APlayerController* UCustomGameInstance::GetFirstAvailableController() const {
	for (int i = 0; i < 2; i++) {
		APlayerController* PC = UGameplayStatics::GetPlayerController(this, i);
		if (IsValid(PC) && PC != BrawlerPlayerController && PC != MarksmanPlayerController) {
			return PC;
		}
	}
	return UGameplayStatics::CreatePlayer(this);
}

void UCustomGameInstance::FallbackRegisterCheckpoint(FCharacterSaveData* TargetSaveData, const FVector& CheckpointLocation, float CheckpointYaw) {
	float PlayerHalfHeight = APlayerCharacter::StaticClass()->GetDefaultObject<APlayerCharacter>()->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	FVector UpdatedLocation = CheckpointLocation + FVector(0, 0, PlayerHalfHeight);
	TargetSaveData->Transform.SetLocation(UpdatedLocation);
		
	TargetSaveData->Transform.SetRotation(FRotator(0, CheckpointYaw, 0).Quaternion());
		
	SaveGameToActiveSlot();
}

void UCustomGameInstance::UpdatePlaytime() {
	if (PlaytimeComparisonValue > 0) {
		ActiveSaveSlot_SaveGame->TotalPlayTime += UGameplayStatics::GetTimeSeconds(this) - PlaytimeComparisonValue;
	}
	
	StartPlaytimeTracking();
}

void UCustomGameInstance::PlayerDiedInCombat() {
	if (MarksmanCharacter_Ref->GetIsDead() && BrawlerCharacter_Ref->GetIsDead()) {
		ApplyFromActiveSlot();
	}
}

void UCustomGameInstance::OnGameplayLevelBeginPlay() {
	StartPlaytimeTracking();
	
	if (!IsValid(ActiveSaveSlot_SaveGame)) {
		ActiveSaveSlot_SaveGame = NewObject<USaveSlot_SaveGame>();
	}
	
	if (ActiveSaveSlot_SaveGame->MarksmanSaveData.Transform.GetLocation() == FVector().ZeroVector) {
		ActiveSaveSlot_SaveGame->MarksmanSaveData.Transform = UHelperFunctions::FindPlayerStart(
			this, MarksmanPlayerController)->GetActorTransform();
	}
	
	if (ActiveSaveSlot_SaveGame->BrawlerSaveData.Transform.GetLocation() == FVector().ZeroVector) {
		ActiveSaveSlot_SaveGame->BrawlerSaveData.Transform = UHelperFunctions::FindPlayerStart(
			this, BrawlerPlayerController)->GetActorTransform();
	}
	
	if (!IsValid(MarksmanCharacter_Ref) || !IsValid(BrawlerCharacter_Ref)) {
		SpawnPlayerCharacters();
	}
}
