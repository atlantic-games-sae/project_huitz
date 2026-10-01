// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "CustomPlayerController.generated.h"

UCLASS()
class PROJECT_HUITZ_API ACustomPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	FName LobbyName = "LobbyName";
	FName SearchKey = "LobbyId";
	FString ConnectString;
	FDelegateHandle JoinSessionDelegateHandle;
	FDelegateHandle FindLobbiesDelegateHandle;
	FDelegateHandle LoginDelegateHandle;
	FOnlineSessionSearchResult* SessionToJoin;
	FDelegateHandle CreateLobbyDelegateHandle;
	
	UFUNCTION(BlueprintCallable, Category="Online Multiplayer")
	void Login();
	void HandleLoginCompleted(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);
	UFUNCTION(BlueprintImplementableEvent, Category = "Online Multiplayer")
	void OnSuccessfulLogin();
	
	UFUNCTION(BlueprintCallable, Category="Online Multiplayer")
	void CreateLobby(FString KeyValue = "KeyValue");
	void HandleCreateLobbyCompleted(FName LobbyName, bool bWasSuccessful);
	UFUNCTION(BlueprintImplementableEvent, Category = "Online Multiplayer")
	void OnLobbyCreated();
	
	UFUNCTION(BlueprintCallable, Category="Online Multiplayer")
	void FindLobby(FString SearchValue = "KeyValue");
	void HandleFindLobbyCompleted(bool bWasSuccessful, TSharedRef<FOnlineSessionSearch> Search);
	UFUNCTION(BlueprintImplementableEvent, Category = "Online Multiplayer")
	void OnLobbyFound();
	
	void JoinLobby();
	void HandleJoinLobbyCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	UFUNCTION(BlueprintImplementableEvent, Category = "Online Multiplayer")
	void OnLobbyJoined();
};
