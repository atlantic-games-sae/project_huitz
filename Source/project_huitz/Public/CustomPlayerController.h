// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "CustomPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNotifyEventDelegate);

UCLASS(BlueprintType)
class PROJECT_HUITZ_API ULobbySearchResultsRow : public UObject {
	GENERATED_BODY()
	
public:
	inline void Initialize(FString PLobbyId, FString POwnerName) {
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
	TArray<ULobbySearchResultsRow*> LobbySearchResultsRows;
	
	UFUNCTION(BlueprintCallable, Category="Online Multiplayer")
	void Login();
	void HandleLoginCompleted(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);
	UPROPERTY(BlueprintAssignable, Category="Online Multiplayer")
	FNotifyEventDelegate OnSuccessfulLogin;
	UPROPERTY(BlueprintAssignable, Category="Online Multiplayer")
	FNotifyEventDelegate OnFailedLogin;
	
	UFUNCTION(BlueprintCallable, Category="Online Multiplayer")
	void CreateLobby(FString LobbyId);
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
};
