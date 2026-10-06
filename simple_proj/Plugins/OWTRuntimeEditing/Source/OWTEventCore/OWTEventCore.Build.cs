using UnrealBuildTool;

public class OWTEventCore : ModuleRules
{
    public OWTEventCore(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicDependencyModuleNames.AddRange(new[] { "Core", "CoreUObject" });
        PrivateDependencyModuleNames.Add("Json");
    }
}
