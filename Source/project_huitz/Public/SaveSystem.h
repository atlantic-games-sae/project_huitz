// © 2026 Atlantic Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "GameFramework/SaveGame.h"
#include "Player/Weapons.h"
#include "SaveSystem.generated.h"

USTRUCT(BlueprintType)
struct FGlobalSettings_SaveData {
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly)
	float MasterVolume = 1.0f;
	UPROPERTY(BlueprintReadOnly)
	float EffectsVolume = 1.0f;
	UPROPERTY(BlueprintReadOnly)
	float MusicVolume = 1.0f;
	
	UPROPERTY(BlueprintReadOnly)
	int GraphicsQualityPreset = 1; // for example - 0,1,2,3 : low,medium,high,ultra
};

USTRUCT(BlueprintType)
struct FPlayerSettings_SaveData {
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly)
	float CameraFieldOfView = 75.0f;
	
	UPROPERTY(BlueprintReadOnly)
	float MouseLookSensitivity = 1.0f;
	UPROPERTY(BlueprintReadOnly)
	float GamepadLookSensitivity = 1.0f;
};

USTRUCT(BlueprintType)
struct FCharacterSaveData {
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly)
	float CurrentHealth = 100.0f;
};

USTRUCT()
struct FBrawlerSaveData : public FCharacterSaveData {
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly)
	TArray<TSubclassOf<UMeleeWeapon>> UnlockedWeapons = {};
};

USTRUCT()
struct FMarksmanSaveData : public FCharacterSaveData {
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly)
	TArray<TSubclassOf<URangedWeapon>> UnlockedWeapons = {};
};

USTRUCT(BlueprintType)
struct FPickupSaveData {
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly)
	TSubclassOf<APickup> Class = APickup::StaticClass();
	UPROPERTY(BlueprintReadOnly)
	FTransform Transform = FTransform();
};

USTRUCT(BlueprintType)
struct FLevelSaveData {
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly)
	TSoftObjectPtr<UWorld> Level;
	
	UPROPERTY(BlueprintReadOnly)
	TArray<FPickupSaveData> ExtraPickups = {}; // Pickups that are not part of the level's default state
	UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<APickup>> CollectedLevelPickups = {}; // Pickups from the level's default state that have been collected already
};

UCLASS()
class PROJECT_HUITZ_API UGlobalSettings_SaveGame : public USaveGame {
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly)
	FGlobalSettings_SaveData GlobalSettings;
	UPROPERTY(BlueprintReadOnly)
	FPlayerSettings_SaveData PrimaryPlayerSettings;
};

UCLASS()
class PROJECT_HUITZ_API USaveSlot_SaveGame : public USaveGame {
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly)
	float TotalPlayTime;
	UPROPERTY(BlueprintReadOnly)
	FPlayerSettings_SaveData SecondaryPlayerSettings;
	UPROPERTY(BlueprintReadOnly)
	FBrawlerSaveData BrawlerSaveData;
	UPROPERTY(BlueprintReadOnly)
	FMarksmanSaveData MarksmanSaveData;
	UPROPERTY(BlueprintReadOnly)
	FLevelSaveData LevelSaveData;
};
