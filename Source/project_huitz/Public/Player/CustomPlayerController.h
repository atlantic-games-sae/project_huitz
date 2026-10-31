// © 2026 Atlantic Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "CustomPlayerController.generated.h"

class UPauseMenuComponent;
class AMainMenuLevelScript;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNotifyEventDelegate);

UCLASS(BlueprintType)
class PROJECT_HUITZ_API ULobbySearchResultsRow : public UObject {
	GENERATED_BODY()
	
public:
	void Initialize(FString PLobbyId, FString POwnerName) {
		LobbyId = PLobbyId;
		OwnerName = POwnerName;
	};
	
	UPROPERTY(BlueprintReadOnly)
	FString LobbyId;
	UPROPERTY(BlueprintReadOnly)
	FString OwnerName;
};

UCLASS()
class PROJECT_HUITZ_API ACustomPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACustomPlayerController(const FObjectInitializer& ObjectInitializer);
	
	void Login();
	void CreateLobby(FString LobbyId);
	
	UPROPERTY(Category="Components", EditDefaultsOnly, BlueprintReadOnly)
	UPauseMenuComponent* PauseMenuComponent;
	
	UPROPERTY(BlueprintAssignable, Category="Online Multiplayer")
	FNotifyEventDelegate OnSuccessfulLogin;

	UPROPERTY(BlueprintAssignable, Category="Online Multiplayer")
	FNotifyEventDelegate OnFailedLogin;
	
protected:
	FName LobbyName = "LobbyName";
	FName SearchKey = "LobbyId";
	FDelegateHandle JoinSessionDelegateHandle;
	FDelegateHandle FindLobbiesDelegateHandle;
	FDelegateHandle LoginDelegateHandle;
	FOnlineSessionSearchResult* LobbyToJoin;
	FString ConnectString;
	FDelegateHandle CreateLobbyDelegateHandle;
	bool bLobbySearchActive;

	FString DesiredLobbyId;
	
	UPROPERTY(BlueprintReadOnly, Category="Online Multiplayer")
	TArray<TObjectPtr<ULobbySearchResultsRow>> LobbySearchResultsRows;
	
	void HandleLoginCompleted(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);
	
	void HandlePreCreateLobbyCheckCompleted(bool bWasSuccessful, TSharedRef<FOnlineSessionSearch> Search);
	void HandleCreateLobbyCompleted(FName LobbyName, bool bWasSuccessful);
	UPROPERTY(BlueprintAssignable, Category="Online Multiplayer")
	FNotifyEventDelegate OnLobbyCreated;
	UPROPERTY(BlueprintAssignable, Category="Online Multiplayer")
	FNotifyEventDelegate OnLobbyCreationFailed;
	
	UFUNCTION(BlueprintCallable, Category="Online Multiplayer")
	void FindLobbies(FString LobbyIdSearch);
	void HandleFindLobbiesCompleted(bool bWasSuccessful, TSharedRef<FOnlineSessionSearch> Search);
	UPROPERTY(BlueprintAssignable, Category="Online Multiplayer")
	FNotifyEventDelegate OnLobbySearchComplete;
	
	UFUNCTION(BlueprintCallable, Category="Online Multiplayer")
	void JoinLobby(int LobbyIndexToJoin);
	void HandleJoinLobbyCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	UPROPERTY(BlueprintAssignable, Category="Online Multiplayer")
	FNotifyEventDelegate OnLobbyJoined;
	
	void FindLobbyToJoin(int LobbyIndexToJoin);
	void HandleFindLobbyToJoinCompleted(bool bWasSuccessful, TSharedRef<FOnlineSessionSearch> Search);
	
private:
	void CreateLobby_Internal();
	void JoinLobby_Internal();
	
	AMainMenuLevelScript* GetMainMenuLevelScript() const;
};
