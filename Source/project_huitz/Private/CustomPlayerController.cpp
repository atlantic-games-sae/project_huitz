// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomPlayerController.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystemTypes.h"
#include "Interfaces/OnlineIdentityInterface.h"

#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionDelegates.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Online/OnlineSessionNames.h"

void ACustomPlayerController::Login() {
	IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
	IOnlineIdentityPtr Identity = Subsystem->GetIdentityInterface();
	
	FUniqueNetIdPtr NetId = Identity->GetUniquePlayerId(0);
	
	if (NetId != nullptr && Identity->GetLoginStatus(0) == ELoginStatus::LoggedIn) return;
	
	LoginDelegateHandle = Identity->AddOnLoginCompleteDelegate_Handle(
		0,
		FOnLoginCompleteDelegate::CreateUObject(
			this,
			&ACustomPlayerController::HandleLoginCompleted));
	
	FOnlineAccountCredentials Credentials(TEXT("AccountPortal"), "", "");
	
	UE_LOG(LogTemp, Log, TEXT("Logging into EOS..."));
	
	if (!Identity->Login(0, Credentials)) {
		UE_LOG(LogTemp, Warning, TEXT("Login failed."));
		Identity->ClearOnLoginCompleteDelegate_Handle(0, LoginDelegateHandle);
		LoginDelegateHandle.Reset();
	}
} 

void ACustomPlayerController::HandleLoginCompleted(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error) {
	IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
	IOnlineIdentityPtr Identity = Subsystem->GetIdentityInterface();

	if (bWasSuccessful) {
		UE_LOG(LogTemp, Log, TEXT("Login callback completed!"));
		if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Logged In!"));
		OnSuccessfulLogin.Broadcast();
	} else { // Login failed
		UE_LOG(LogTemp, Warning, TEXT("EOS login failed.")); //Print sign in failure in logs as a warning.
		if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Failed to Log In"));
		OnFailedLogin.Broadcast();
	}

	Identity->ClearOnLoginCompleteDelegate_Handle(LocalUserNum, LoginDelegateHandle);
	LoginDelegateHandle.Reset();
}

void ACustomPlayerController::CreateLobby(FString LobbyId) {
	if (bLobbySearchActive) return;
	
	if (LobbyId == FString("")) {
		UE_LOG(LogTemp, Warning, TEXT("LobbyId cannot be empty string."));
		if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("LobbyId cannot be empty string."));
		return;
	}
	
	DesiredLobbyId = LobbyId;
	
    IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
    IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
	TSharedRef<FOnlineSessionSearch> Search = MakeShared<FOnlineSessionSearch>();
 
	// Remove the default search parameters that FOnlineSessionSearch sets up.
	Search->QuerySettings.SearchParams.Empty();
 
	Search->QuerySettings.Set(SearchKey, DesiredLobbyId, EOnlineComparisonOp::Equals); // Search using our Key/Value pair
	Search->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);
	
	FindLobbiesDelegateHandle = Session->AddOnFindSessionsCompleteDelegate_Handle(
        FOnFindSessionsCompleteDelegate::CreateUObject(
            this,
            &ThisClass::HandlePreCreateLobbyCheckCompleted,
            Search));
	
	bLobbySearchActive = true;
 
	if (!Session->FindSessions(0, Search)) {
		CreateLobby_Internal();
		Session->ClearOnFindSessionsCompleteDelegate_Handle(FindLobbiesDelegateHandle);
		FindLobbiesDelegateHandle.Reset();
	}
}

void ACustomPlayerController::HandlePreCreateLobbyCheckCompleted(bool bWasSuccessful, TSharedRef<FOnlineSessionSearch> Search) {
	IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
	IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
	if (!bWasSuccessful) CreateLobby_Internal();
	else {
		if (Search->SearchResults.Num() == 0) CreateLobby_Internal();
	}
	
	bLobbySearchActive = false;
	
	Session->ClearOnFindSessionsCompleteDelegate_Handle(FindLobbiesDelegateHandle);
	FindLobbiesDelegateHandle.Reset();
}

void ACustomPlayerController::CreateLobby_Internal() {
	IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
	IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
	CreateLobbyDelegateHandle =
		Session->AddOnCreateSessionCompleteDelegate_Handle(FOnCreateSessionCompleteDelegate::CreateUObject(
			this,
			&ThisClass::HandleCreateLobbyCompleted));
	
	// These are the settings for the lobby
	TSharedRef<FOnlineSessionSettings> SessionSettings = MakeShared<FOnlineSessionSettings>();
	SessionSettings->NumPublicConnections = 2; // We will test our sessions with 2 players to keep things simple
	SessionSettings->bShouldAdvertise = true; // This creates a public match and will be searchable.
	SessionSettings->bUsesPresence = false;   // No presence on dedicated server. This requires a local user.
	SessionSettings->bAllowJoinViaPresence = false;
	SessionSettings->bAllowJoinViaPresenceFriendsOnly = false;
	SessionSettings->bAllowInvites = false;    // Allow inviting players into session. This requires presence and a local user. 
	SessionSettings->bAllowJoinInProgress = false; // Once the session is started, no one can join.
	SessionSettings->bIsDedicated = false; // Session created on dedicated server.
	SessionSettings->bUseLobbiesIfAvailable = true; // For P2P we will use a lobby instead of a session
	SessionSettings->bUseLobbiesVoiceChatIfAvailable = true; // We will also enable voice
	SessionSettings->bUsesStats = true; // Needed to keep track of player stats.
	SessionSettings->Settings.Add(SearchKey, FOnlineSessionSetting((DesiredLobbyId), EOnlineDataAdvertisementType::ViaOnlineService));
		
	UE_LOG(LogTemp, Warning, TEXT("Creating Lobby..."));
	if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Creating Lobby..."));
	 
	if (!Session->CreateSession(0, LobbyName, *SessionSettings)) {
		UE_LOG(LogTemp, Warning, TEXT("Failed to create Lobby!"));
		if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Failed to create Lobby!"));
		
		Session->ClearOnCreateSessionCompleteDelegate_Handle(CreateLobbyDelegateHandle);
		CreateLobbyDelegateHandle.Reset();
	}
}

void ACustomPlayerController::HandleCreateLobbyCompleted(FName EOSLobbyName, bool bWasSuccessful) {
    // This is called once our lobby is created
 
    IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
    IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
    if (bWasSuccessful) {
        UE_LOG(LogTemp, Warning, TEXT("Lobby: %s Created!"), *EOSLobbyName.ToString());
        if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Lobby Created!!"));
        FString Map = "Game/Content/TestingLevels/MovementGym?listen"; // TODO: remove hardcoded map file location
        FURL TravelURL;
        TravelURL.Map = Map;
        GetWorld()->Listen(TravelURL);
    	OnLobbyCreated.Broadcast();
    } else {
        UE_LOG(LogTemp, Warning, TEXT("Failed to create lobby!"));
        if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Failed to create Lobby!"));
    }
 
    // Clear our handle and reset the delegate. 
    Session->ClearOnCreateSessionCompleteDelegate_Handle(CreateLobbyDelegateHandle);
    CreateLobbyDelegateHandle.Reset();
}

void ACustomPlayerController::FindLobbies(FString LobbyIdSearch) { // Put default value for example
	if (bLobbySearchActive) return;
	
    IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
    IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
    TSharedRef<FOnlineSessionSearch> Search = MakeShared<FOnlineSessionSearch>();
 
    // Remove the default search parameters that FOnlineSessionSearch sets up.
    Search->QuerySettings.SearchParams.Empty();
 
	// Search using our input if it is not an empty string, otherwise search for all lobbies
	if (LobbyIdSearch != FString("")) Search->QuerySettings.Set(SearchKey, LobbyIdSearch, EOnlineComparisonOp::Equals);
	
    Search->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);
    UE_LOG(LogTemp, Display, TEXT("Finding lobby..."));
	if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Finding lobby..."));
    
    FindLobbiesDelegateHandle = Session->AddOnFindSessionsCompleteDelegate_Handle(
    	FOnFindSessionsCompleteDelegate::CreateUObject(
            this,
            &ThisClass::HandleFindLobbiesCompleted,
            Search));
 
	bLobbySearchActive = true;
    if (!Session->FindSessions(0, Search)) {
        //UE_LOG(LogTemp, Warning, TEXT("Finding lobby failed."));
        //if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Finding lobby failed."));

        // Clear our handle and reset the delegate. 
        Session->ClearOnFindSessionsCompleteDelegate_Handle(FindLobbiesDelegateHandle);
        FindLobbiesDelegateHandle.Reset();
    }
}
 
void ACustomPlayerController::HandleFindLobbiesCompleted(bool bWasSuccessful, TSharedRef<FOnlineSessionSearch> Search) {
    IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
    IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
 
    if (bWasSuccessful && Search->SearchResults.Num() > 0) {
    	UE_LOG(LogTemp, Warning, TEXT("Search results cleared"));
    	LobbySearchResultsRows.Empty(); // Clear search results arrays
    	
        for (auto SessionInSearchResult : Search->SearchResults) {
        	FString LobbyId = SessionInSearchResult.Session.SessionSettings.Settings.Find(SearchKey)->Data.ToString();
        	FString DisplayName = SessionInSearchResult.Session.OwningUserName;
        	
        	UE_LOG(LogTemp, Display, TEXT("LobbyId: %s, OwnerName: %s"), *LobbyId, *DisplayName);
        	
        	FString ConnectStringTemp;
            // Ensure the connection string is resolvable and store the info in ConnectString and in SessionToJoin
            if (Session->GetResolvedConnectString(SessionInSearchResult, NAME_GamePort, ConnectStringTemp) && DisplayName != "") {
            	ULobbySearchResultsRow* RowObject = NewObject<ULobbySearchResultsRow>(this);
            	RowObject->Initialize(LobbyId, DisplayName);
            	LobbySearchResultsRows.Add(RowObject);
            }
        }
    } else {
        UE_LOG(LogTemp, Warning, TEXT("Find lobby failed."));
		if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("No lobbies found!"));
    }
 
	bLobbySearchActive = false;
	OnLobbySearchComplete.Broadcast();
	
    // Clear our handle and reset the delegate.
    Session->ClearOnFindSessionsCompleteDelegate_Handle(FindLobbiesDelegateHandle);
    FindLobbiesDelegateHandle.Reset();
}

void ACustomPlayerController::JoinLobby(int LobbyIndexToJoin) {
	if (LobbyIndexToJoin < 0 || LobbyIndexToJoin >= LobbySearchResultsRows.Num()) return;
	
	FindLobbyToJoin(LobbyIndexToJoin);
}

void ACustomPlayerController::FindLobbyToJoin(int LobbyIndexToJoin) {
	if (bLobbySearchActive) return;
	
	IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
	IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
	TSharedRef<FOnlineSessionSearch> Search = MakeShared<FOnlineSessionSearch>();
 
	// Remove the default search parameters that FOnlineSessionSearch sets up.
	Search->QuerySettings.SearchParams.Empty();

	Search->QuerySettings.Set(SearchKey, LobbySearchResultsRows[LobbyIndexToJoin]->LobbyId, EOnlineComparisonOp::Equals);
	Search->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);
	
	//UE_LOG(LogTemp, Display, TEXT("Finding lobby..."));
	//if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Finding lobby..."));
    
	FindLobbiesDelegateHandle = Session->AddOnFindSessionsCompleteDelegate_Handle(
		FOnFindSessionsCompleteDelegate::CreateUObject(
			this,
			&ThisClass::HandleFindLobbyToJoinCompleted,
			Search));
 
	bLobbySearchActive = true;
	if (!Session->FindSessions(0, Search)) {
		//UE_LOG(LogTemp, Warning, TEXT("Finding lobby failed."));
		//if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Finding lobby failed."));

		// Clear our handle and reset the delegate. 
		Session->ClearOnFindSessionsCompleteDelegate_Handle(FindLobbiesDelegateHandle);
		FindLobbiesDelegateHandle.Reset();
	}
}

void ACustomPlayerController::HandleFindLobbyToJoinCompleted(bool bWasSuccessful, TSharedRef<FOnlineSessionSearch> Search) {
	IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
	IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
 
	if (bWasSuccessful) {
		if (Search->SearchResults.Num() == 0) {
			UE_LOG(LogTemp, Warning, TEXT("No lobbies found."));
			if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("No lobbies found!"));
		} else {
			UE_LOG(LogTemp, Warning, TEXT("Found lobby."));
			for (auto SessionInSearchResult : Search->SearchResults) {
				// Ensure the connection string is resolvable and store the info in ConnectString and in LobbyToJoin
				if (Session->GetResolvedConnectString(SessionInSearchResult, NAME_GamePort, ConnectString)) {
					LobbyToJoin = &SessionInSearchResult;
					JoinLobby_Internal();
					break;
				}
			}
		}
	} else {
		UE_LOG(LogTemp, Warning, TEXT("Find lobby failed."));
		if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("No lobbies found!"));
	}
 
	// Clear our handle and reset the delegate. 
	Session->ClearOnFindSessionsCompleteDelegate_Handle(FindLobbiesDelegateHandle);
	FindLobbiesDelegateHandle.Reset();
}

void ACustomPlayerController::JoinLobby_Internal() {
	IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
	IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
 
	JoinSessionDelegateHandle = 
		Session->AddOnJoinSessionCompleteDelegate_Handle(FOnJoinSessionCompleteDelegate::CreateUObject(
			this,
			&ThisClass::HandleJoinLobbyCompleted));

	UE_LOG(LogTemp, Warning, TEXT("Joining Lobby."));
	if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Joining Lobby!"));
	
	if (!Session->JoinSession(0, "SessionName", *LobbyToJoin)) {
		UE_LOG(LogTemp, Warning, TEXT("Join Lobby failed."));
		if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Join Lobby failed!"));

		// Clear our handle and reset the delegate.
		Session->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionDelegateHandle);
		JoinSessionDelegateHandle.Reset();
	}
}
 
void ACustomPlayerController::HandleJoinLobbyCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result) {
	// Tutorial 4: This function is triggered via the callback we set in JoinSession once the session is joined (or there is a failure)
	
	IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
	IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
	if (Result == EOnJoinSessionCompleteResult::Success) {
		UE_LOG(LogTemp, Warning, TEXT("Joined lobby."));
		if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Joined lobby!"));

		ClientTravel(ConnectString, TRAVEL_Absolute);
		OnLobbyJoined.Broadcast();
	}
 
	// Clear our handle and reset the delegate. 
	Session->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionDelegateHandle);
	JoinSessionDelegateHandle.Reset();
}
