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
	FString ConnectString;
	FDelegateHandle JoinSessionDelegateHandle;
	FDelegateHandle FindLobbiesDelegateHandle;
	FDelegateHandle LoginDelegateHandle;
	FOnlineSessionSearchResult* SessionToJoin;
	FDelegateHandle CreateLobbyDelegateHandle;
	
	virtual void BeginPlay() override;
	void Login();
	void HandleLoginCompleted(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);
	void CreateLobby(FName KeyName = "KeyName", FString KeyValue = "KeyValue");
	void HandleCreateLobbyCompleted(FName LobbyName, bool bWasSuccessful);
	void FindLobby(FName SearchKey = "KeyName", FString SearchValue = "KeyValue");
	void HandleFindLobbyCompleted(bool bWasSuccessful, TSharedRef<FOnlineSessionSearch> Search);
	void JoinLobby();
	void HandleJoinLobbyCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
};
