#pragma once
#include "CoreMinimal.h"
#include "IDetailCustomization.h"
#include "IPropertyUtilities.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"

//Forward declaration of IPropertyHandle.
class IPropertyHandle;

//Custom Class Details Customization
class FCustomMovementComponentDetailsCustomization : public IDetailCustomization {
public:
	//Function that customizes the Details Panel.
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

	void HidePropertiesInCategory(IDetailLayoutBuilder& DetailBuilder, FName CategoryName, TArray<FString> ExcludedProperties);
	void HideCategoryExcludingProperties(IDetailLayoutBuilder& DetailBuilder, FName CategoryName, TArray<FString> ExcludedProperties);

	//Returns a static instance of the Details Panel customization.
	static TSharedRef<IDetailCustomization> MakeInstance();

	static void SortCustomDetailsCategories(const TMap<FName, IDetailCategoryBuilder*>& AllCategoryMap);
};
