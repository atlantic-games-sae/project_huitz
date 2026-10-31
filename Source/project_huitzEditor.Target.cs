// © 2026 Atlantic Games. All rights reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class project_huitzEditorTarget : TargetRules
{
	public project_huitzEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "project_huitz" } );
	}
}
