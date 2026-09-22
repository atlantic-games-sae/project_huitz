using UnrealBuildTool;
	 
public class CustomEditor: ModuleRules
{
    public CustomEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PrivateDependencyModuleNames.AddRange(new string[] {"Core", "CoreUObject", "Engine", "project_huitz", "Slate", "SlateCore"});
    }
}
