// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Marooned : ModuleRules
{
	public Marooned(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "AssetRegistry", "Json", "JsonUtilities"});

		PublicIncludePaths.AddRange(
			new string[] {
				"Marooned",
				"Marooned/Crafting"
			});
	}
}
