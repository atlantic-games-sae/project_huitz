// © 2026 Atlantic Games. All rights reserved.

#include "Libraries/CollisionLibrary.h"

#include "KismetTraceUtils.h"

// This function is from https://dev.epicgames.com/community/learning/tutorials/pBv0/how-to-perform-cone-shaped-traces-in-unreal-engine
// Explanation of its functionality at https://sreitich.github.io/cone-trace
// This code is not explicitly licensed by its author, but I have decided it is okay to include in this academic, non-commercial context
// In any other context, I would ask the author's permission to use this code in my game
bool UCollisionLibrary::ConeTraceMulti(
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
	FLinearColor TraceColor,
	FLinearColor TraceHitColor,
	float DrawTime)
{
	OutHits.Reset();

	ECollisionChannel CollisionChannel = UEngineTypes::ConvertToCollisionChannel(TraceChannel);
	FCollisionQueryParams Params(SCENE_QUERY_STAT(ConeTraceMulti), bTraceComplex);
	Params.bReturnPhysicalMaterial = true;
	Params.AddIgnoredActors(ActorsToIgnore);

	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World)
	{
		return false;
	}

	TArray<FHitResult> TempHitResults;
	const FVector End = Start + (Direction.Vector() * ConeHeight);
	const double ConeHalfAngleRad = FMath::DegreesToRadians(ConeHalfAngle);
	// r = h * tan(theta / 2)
	const double ConeBaseRadius = ConeHeight * tan(ConeHalfAngleRad);
	const FCollisionShape SphereSweep = FCollisionShape::MakeSphere(ConeBaseRadius);

	// Perform a sweep encompassing an imaginary cone.
	World->SweepMultiByChannel(TempHitResults, Start, End, Direction.Quaternion(), CollisionChannel, SphereSweep, Params);

	// Filter for hits that would be inside the cone.
	for (FHitResult& HitResult : TempHitResults)
	{
		const FVector HitDirection = (HitResult.ImpactPoint - Start).GetSafeNormal();
		const double Dot = FVector::DotProduct(Direction.Vector(), HitDirection);
		// theta = arccos((A • B) / (|A|*|B|)). |A|*|B| = 1 because A and B are unit vectors.
		const double DeltaAngle = FMath::Acos(Dot);

		// Hit is outside the angle of the cone.
		if (DeltaAngle > ConeHalfAngleRad)
		{
			continue;
		}

		const double Distance = (HitResult.ImpactPoint - Start).Length();
		// Hypotenuse = adjacent / cos(theta)
		const double LengthAtAngle = ConeHeight / cos(DeltaAngle);

		// Hit is beyond the cone. This can happen because we sweep with spheres, which results in a cap at the end of the sweep.
		if (Distance > LengthAtAngle)
		{
			continue;
		}

		OutHits.Add(HitResult);
	}

#if ENABLE_DRAW_DEBUG
	if (DrawDebugType != EDrawDebugTrace::None)
	{
		// Cone trace.
		const double ConeSlantHeight = FMath::Sqrt((ConeBaseRadius * ConeBaseRadius) + (ConeHeight * ConeHeight)); // s = sqrt(r^2 + h^2)
		DrawDebugCone(World, Start, Direction.Vector(), ConeSlantHeight, ConeHalfAngleRad, ConeHalfAngleRad, 32, TraceColor.ToFColor(true), (DrawDebugType == EDrawDebugTrace::Persistent), DrawTime);

		// Uncomment to see the trace we're actually performing.
		// DrawDebugSweptSphere(World, Start, End, ConeBaseRadius, TraceColor.ToFColor(true), (DrawDebugType == EDrawDebugTrace::Persistent), DrawTime);

		// Successful hits.
		for (const FHitResult& Hit : OutHits)
		{
			DrawDebugLineTraceSingle(World, Hit.TraceStart, Hit.ImpactPoint, DrawDebugType, true, Hit, TraceHitColor, TraceHitColor, DrawTime);
		}

		// Uncomment to see hits from the sphere sweep that were filtered out.
		// for (const FHitResult& Hit : TempHitResults)
		// {
		//     if (!OutHits.ContainsByPredicate([Hit](const FHitResult& Other)
		//     {
		//         return (Hit.GetActor() == Other.GetActor()) &&
		//                (Hit.ImpactPoint == Other.ImpactPoint) &&
		//                (Hit.ImpactNormal == Other.ImpactNormal);
		//     }))
		//     {
		//         DrawDebugLineTraceSingle(World, Hit.TraceStart, Hit.ImpactPoint, DrawDebugType, false, Hit, FColor::Red, FColor::Red, DrawTime);
		//     }
		// }
	}
#endif // ENABLE_DRAW_DEBUG

	return (OutHits.Num() > 0);
}
