#pragma once

#include "CoreMinimal.h"
#include "CustomGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "HelperFunctions.generated.h"

UCLASS()
class PROJECT_HUITZ_API UHelperFunctions : public UObject {
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintPure, Category="Helper Functions")
    static float GetAlphaInRange(float Input, float Min, float Max) {
        float* RealMin;
        float* RealMax;
        if (Min < Max) {
            RealMin = &Min;
            RealMax = &Max;
        } else {
            RealMin = &Max;
            RealMax = &Min;
        }
        float ClampedInput = FMath::Clamp(Input, *RealMin, *RealMax);
        float InternalResult = (ClampedInput - *RealMin) / (*RealMax - *RealMin);

        if (Max < Min) return 1.0f - InternalResult;
        return InternalResult;
    }

    UFUNCTION(BlueprintPure, Category="Helper Functions")
    static float FloatMoveTowards(float BaseValue, float TargetValue, float MaxDelta) {
        if (BaseValue == TargetValue) return BaseValue;
        if (MaxDelta >= FMath::Abs(BaseValue - TargetValue)) return TargetValue;
        if (TargetValue > BaseValue) return BaseValue + MaxDelta;
        return BaseValue - MaxDelta;
    }
    
    UFUNCTION(BlueprintPure, Category="Helper Functions")
    static FVector2D MovementInputToWorldSpace(FVector2D Input, float ActorYaw) {
        FVector2D NormalInput = Input.GetSafeNormal();
        return FVector2D(NormalInput.X, -NormalInput.Y).GetRotated(ActorYaw + 90.0).GetSafeNormal();
    }
    
    UFUNCTION(BlueprintPure, Category="Helper Functions")
    static UCustomGameInstance* GetCustomGameInstance(const UObject* WorldContextObject) {
        return static_cast<UCustomGameInstance*>(UGameplayStatics::GetGameInstance(WorldContextObject));
    }
};
