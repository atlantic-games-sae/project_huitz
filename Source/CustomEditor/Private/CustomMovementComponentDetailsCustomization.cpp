#include "Widgets/SWidget.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "HAL/PlatformApplicationMisc.h"
#include "CustomMovementComponent.h"
#include "CustomMovementComponentDetailsCustomization.h"

void FCustomMovementComponentDetailsCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
    HideCategoryExcludingProperties(DetailBuilder, FName("Character Movement (General Settings)"), { FString("GravityScale"), FString("Mass") });
    HideCategoryExcludingProperties(DetailBuilder, FName("Character Movement: Walking"), { FString("MaxStepHeight"), FString("WalkableFloorAngle"), FString("GroundFriction") });
    HideCategoryExcludingProperties(DetailBuilder, FName("Character Movement: Jumping / Falling"), { FString("JumpZVelocity"), FString("AirControl"), FString("BrakingDecelerationFalling") });

    DetailBuilder.HideCategory(FName("Character Movement: Gravity"));
    DetailBuilder.HideCategory(FName("Character Movement: Swimming"));
    DetailBuilder.HideCategory(FName("Character Movement: Flying"));
    DetailBuilder.HideCategory(FName("Character Movement: Custom Movement"));
    DetailBuilder.HideCategory(FName("Velocity"));
}

void FCustomMovementComponentDetailsCustomization::HidePropertiesInCategory(IDetailLayoutBuilder& DetailBuilder, FName CategoryName, TArray<FString> TargetProperties) {
    IDetailCategoryBuilder& TargetCategory = DetailBuilder.EditCategory(CategoryName);
    TArray<TSharedRef<IPropertyHandle>> Properties;
    TargetCategory.GetDefaultProperties(Properties);
    for (int i = 0; i < Properties.Num(); i++) {
        bool HasMatched = false;

        for (int j = 0; j < TargetProperties.Num() && !HasMatched; j++) {
            if (Properties[i].Get().GeneratePathToProperty().EndsWith(TargetProperties[j], ESearchCase::CaseSensitive)) HasMatched = true;
        }
        
        if (HasMatched) DetailBuilder.HideProperty(Properties[i]);
    }
}

void FCustomMovementComponentDetailsCustomization::HideCategoryExcludingProperties(IDetailLayoutBuilder& DetailBuilder, FName CategoryName, TArray<FString> ExcludedProperties) {
    IDetailCategoryBuilder& TargetCategory = DetailBuilder.EditCategory(CategoryName);
    TArray<TSharedRef<IPropertyHandle>> Properties;
    TargetCategory.GetDefaultProperties(Properties);
    for (int i = 0; i < Properties.Num(); i++) {
        bool HasMatched = false;

        for (int j = 0; j < ExcludedProperties.Num() && !HasMatched; j++) {
            if (Properties[i].Get().GeneratePathToProperty().EndsWith(ExcludedProperties[j], ESearchCase::CaseSensitive)) HasMatched = true;
        }
        
        if (!HasMatched) DetailBuilder.HideProperty(Properties[i]);
    }
}

void FCustomMovementComponentDetailsCustomization::SortCustomDetailsCategories(const TMap<FName, IDetailCategoryBuilder*>& AllCategoryMap) {
    (*AllCategoryMap.Find(FName("Custom Character Movement|Movement Speed")))->SetSortOrder(0);
    (*AllCategoryMap.Find(FName("Custom Character Movement|Acceleration & Deceleration")))->SetSortOrder(1);
    (*AllCategoryMap.Find(FName("Custom Character Movement|Jumping & Gravity")))->SetSortOrder(2);
    (*AllCategoryMap.Find(FName("Custom Character Movement|Wall Jumping")))->SetSortOrder(3);
    (*AllCategoryMap.Find(FName("Custom Character Movement|Ledge Mantling")))->SetSortOrder(4);
}

//Create the static instance of this detail customization needed for registering it in module startup.
TSharedRef<IDetailCustomization> FCustomMovementComponentDetailsCustomization::MakeInstance()
{
    return MakeShareable(new FCustomMovementComponentDetailsCustomization);
}
