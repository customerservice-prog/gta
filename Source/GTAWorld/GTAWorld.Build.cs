using UnrealBuildTool;

public class GTAWorld : ModuleRules
{
	public GTAWorld(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"HeadMountedDisplay",
			"AIModule",
			"NavigationSystem",
			"PhysicsCore",
			"GameplayTags",
			"GameplayTasks",
			"EnhancedInput",
			"ChaosVehicles"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Slate",
			"SlateCore",
			"UMG",
			"Renderer",
			"RHI",
			"RenderCore"
		});
	}
}
