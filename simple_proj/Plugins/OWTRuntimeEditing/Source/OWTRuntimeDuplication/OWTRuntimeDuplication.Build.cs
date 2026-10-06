using UnrealBuildTool;

public class OWTRuntimeDuplication : ModuleRules
{
    public OWTRuntimeDuplication(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicDependencyModuleNames.AddRange(new[] { "Core", "CoreUObject", "Engine" });
    }
}
