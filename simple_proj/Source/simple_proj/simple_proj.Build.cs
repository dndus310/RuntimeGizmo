using UnrealBuildTool;

public class simple_proj : ModuleRules
{
	public simple_proj(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PublicIncludePaths.Add(ModuleDirectory);
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput", 
			"InteractiveToolsFramework",
			"VTBOWTEditor",
			"ModularGameplay"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Slate", 
			"SlateCore"
		});

	}
}
