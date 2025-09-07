// ©Kester McPherson. All rights reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class BullCowGameTarget : TargetRules
{
	public BullCowGameTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "BullCowGame" } );
	}
}
