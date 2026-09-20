// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class simple_proj : ModuleRules
{
	public simple_proj(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		// Keep module-relative includes across the runtime editor folders.
		PublicIncludePaths.Add(ModuleDirectory);
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput", 
			"InteractiveToolsFramework",
			"VTBRuntimeEditor",
			"ModularGameplay"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Slate", 
			"SlateCore"
		});

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
