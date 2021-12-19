// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BulkRenamer : ModuleRules
{
	public BulkRenamer(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
		}
		);
        
		PrivateDependencyModuleNames.AddRange(new string[]
		{
            "CoreUObject",
            "EditorStyle",
            "Engine",
            "InputCore",
            "Json",
            "JsonUtilities",
            "LevelEditor",
            "Projects",
            "Slate",
            "SlateCore",
            "UnrealEd",
        }
		);
    }
}
