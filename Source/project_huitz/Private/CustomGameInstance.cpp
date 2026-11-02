// © 2026 Atlantic Games. All rights reserved.

#include "CustomGameInstance.h"
#include "SaveSystem.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "LevelScripts/GameplayLevelScript.h"
#include "Player/PlayerCharacter.h"

void UCustomGameInstance::SaveGameToActiveSlot() {
	if (!IsValid(ActiveSaveSlot_SaveGame)) {
		ActiveSaveSlot_SaveGame = NewObject<USaveSlot_SaveGame>();
	}
	
	ActiveSaveSlot_SaveGame->LevelSaveData.CollectedLevelPickups = {};
	ActiveSaveSlot_SaveGame->LevelSaveData.ExtraPickups = {};
	
	ActiveSaveSlot_SaveGame->TotalPlayTime += UGameplayStatics::GetTimeSeconds(this) - TimestampOfLastSave;
	UpdateTimestampOfLastSave();
	
	// Brawler and marksman save data should be gathered and added here
	if (IsValid(MarksmanCharacter_Ref)) {
		ActiveSaveSlot_SaveGame->MarksmanSaveData.Transform = MarksmanCharacter_Ref->GetActorTransform();
	}
	
	if (IsValid(BrawlerCharacter_Ref)) {
		ActiveSaveSlot_SaveGame->BrawlerSaveData.Transform = BrawlerCharacter_Ref->GetActorTransform();
	}
	
	UGameplayStatics::SaveGameToSlot(ActiveSaveSlot_SaveGame, SaveSlotNames[ActiveSaveSlotIndex], 0);
}

void UCustomGameInstance::LoadGameFromActiveSlot() {
	ActiveSaveSlot_SaveGame = Cast<USaveSlot_SaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotNames[ActiveSaveSlotIndex], 0));
	
	UpdateTimestampOfLastSave();
	
	if (!IsValid(ActiveSaveSlot_SaveGame)) {
		ActiveSaveSlot_SaveGame = NewObject<USaveSlot_SaveGame>();
		ActiveSaveSlot_SaveGame->LevelSaveData.Level = GameLevels[0];
		UGameplayStatics::OpenLevelBySoftObjectPtr(this, ActiveSaveSlot_SaveGame->LevelSaveData.Level.LoadSynchronous());
		SaveGameToActiveSlot();
	} else {
		UGameplayStatics::OpenLevelBySoftObjectPtr(this, ActiveSaveSlot_SaveGame->LevelSaveData.Level.LoadSynchronous());
	}
	
	AGameplayLevelScript* GameplayLevelScript = Cast<AGameplayLevelScript>(GetWorld()->GetCurrentLevel()->GetLevelScriptActor());
	if (!IsValid(GameplayLevelScript)) return;
	GameplayLevelScript->LoadFromLevelSaveData(ActiveSaveSlot_SaveGame->LevelSaveData);
}

void UCustomGameInstance::ClearSaveSlot(int SaveSlotIndex) {
	UGameplayStatics::DeleteGameInSlot(SaveSlotNames[SaveSlotIndex], 0);
}

void UCustomGameInstance::SpawnPlayerCharacters(APlayerController* MarksmanPlayerController, APlayerController* BrawlerPlayerController) {
	AActor* PlayerStart;
	FVector Location;
	FRotator Rotation;
	FActorSpawnParameters SpawnParams;
	
	APlayerController* MarksmanPlayerController_Local = IsValid(MarksmanPlayerController) ? MarksmanPlayerController : UGameplayStatics::CreatePlayer(this);
	APlayerController* BrawlerPlayerController_Local = IsValid(BrawlerPlayerController) ? BrawlerPlayerController : UGameplayStatics::CreatePlayer(this);
	
	if (!IsValid(MarksmanCharacter_Ref)) {
		PlayerStart = GetWorld()->GetAuthGameMode()->FindPlayerStart(MarksmanPlayerController_Local);
		
		Location = PlayerStart->GetActorLocation();
		Rotation = PlayerStart->GetActorRotation();
		MarksmanCharacter_Ref = Cast<APlayerCharacter>(GetWorld()->SpawnActor(MarksmanCharacterClass, &Location, &Rotation, SpawnParams));
	}
	
	if (!IsValid(BrawlerCharacter_Ref)) {
		PlayerStart = GetWorld()->GetAuthGameMode()->FindPlayerStart(BrawlerPlayerController_Local);
		
		Location = PlayerStart->GetActorLocation();
		Rotation = PlayerStart->GetActorRotation();
		BrawlerCharacter_Ref = Cast<APlayerCharacter>(GetWorld()->SpawnActor(BrawlerCharacterClass, &Location, &Rotation, SpawnParams));
	}
	
	MarksmanPlayerController_Local->Possess(MarksmanCharacter_Ref);
	
	BrawlerPlayerController_Local->Possess(BrawlerCharacter_Ref);
}

void UCustomGameInstance::SpawnPlayerCharacters(int MarksmanPlayerControllerIndex, int BrawlerPlayerControllerIndex) {
	SpawnPlayerCharacters(
		UGameplayStatics::GetPlayerController(this, MarksmanPlayerControllerIndex),
		UGameplayStatics::GetPlayerController(this, BrawlerPlayerControllerIndex));
}

void UCustomGameInstance::UpdateTimestampOfLastSave() {
	TimestampOfLastSave = UGameplayStatics::GetTimeSeconds(this);
}
