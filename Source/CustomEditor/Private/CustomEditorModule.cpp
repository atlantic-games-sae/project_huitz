#include "CustomEditorModule.h"
#include "CustomMovementComponentDetailsCustomization.h"
#include "CustomMovementComponent.h"
	 
IMPLEMENT_GAME_MODULE(FCustomEditorModule, CustomEditor);

void FCustomEditorModule::StartupModule() {
    FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

    PropertyModule.RegisterCustomClassLayout(UCustomMovementComponent::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FCustomMovementComponentDetailsCustomization::MakeInstance));
}

void FCustomEditorModule::ShutdownModule() {
    FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

    PropertyModule.UnregisterCustomClassLayout(UCustomMovementComponent::StaticClass()->GetFName());
}
