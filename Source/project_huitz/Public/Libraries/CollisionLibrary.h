// © 2026 Atlantic Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UObject/Object.h"
#include "CollisionLibrary.generated.h"

UCLASS()
class PROJECT_HUITZ_API UCollisionLibrary : public UObject {
	GENERATED_BODY()

public:
	UFUNCTION(
		BlueprintCallable,
		Category = "Collision",
		Meta = (
			bIgnoreSelf = "true",
			WorldContext = "WorldContextObject",
			AutoCreateRefTerm = "ActorsToIgnore",
			DisplayName = "Multi Cone Trace By Channel",
			AdvancedDisplay = "TraceColor, TraceHitColor, DrawTime",
			Keywords = "sweep"))
	static bool ConeTraceMulti(
		const UObject* WorldContextObject,
		const FVector Start,
		const FRotator Direction,
		float ConeHeight,
		float ConeHalfAngle,
		ETraceTypeQuery TraceChannel,
		bool bTraceComplex,
		const TArray<AActor*>& ActorsToIgnore,
		EDrawDebugTrace::Type DrawDebugType,
		TArray<FHitResult>& OutHits,
		FLinearColor TraceColor = FLinearColor::Red,
		FLinearColor TraceHitColor = FLinearColor::Green,
		float DrawTime = 5.0f);
};
