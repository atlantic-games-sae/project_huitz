// © 2026 Atlantic Games. All rights reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class project_huitzTarget : TargetRules
{
	public project_huitzTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "project_huitz" } );
	}
}
