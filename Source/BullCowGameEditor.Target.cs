// ©Kester McPherson. All rights reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class BullCowGameEditorTarget : TargetRules
{
	public BullCowGameEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "BullCowGame" } );
	}
}
