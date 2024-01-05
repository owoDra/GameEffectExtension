// Copyright (C) 2023 owoDra

using UnrealBuildTool;

public class GFXExtEditor : ModuleRules
{
    public GFXExtEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(
            new string[]
            {
                ModuleDirectory,
                ModuleDirectory + "/GFXExtEditor",
            }
        );


        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "UnrealEd",
                "AssetTools",
            }
        );


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "GFXExt",
            }
        );
    }
}
