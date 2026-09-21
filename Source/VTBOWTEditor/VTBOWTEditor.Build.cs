using UnrealBuildTool;

public class VTBOWTEditor : ModuleRules
{
    public VTBOWTEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "InteractiveToolsFramework",
                "EnhancedInput",
                "InputCore",
                "CommonUI",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "GeometryFramework",
                "Slate",
                "SlateCore"
            }
        );
    }
}
