using UnrealBuildTool;

public class VTBOWTEditorAutomation : ModuleRules
{
    public VTBOWTEditorAutomation(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PrivateDependencyModuleNames.AddRange(new[]
        {
            "Core", "CoreUObject", "Engine", "UnrealEd", "BlueprintGraph",
            "KismetCompiler", "AssetRegistry", "VTBOWTEditor", "InteractiveToolsFramework", "EnhancedInput", "InputCore", "InputBlueprintNodes"
        });
    }
}

