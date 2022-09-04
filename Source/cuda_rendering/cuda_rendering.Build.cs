// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class cuda_rendering : ModuleRules
{
	private string poject_root_path
	{
		get { return Path.Combine(ModuleDirectory, "../.."); }
	}

	public cuda_rendering(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		string custom_cuda_lib_include = "CUDALib/include";
		string custom_cuda_lib_lib = "CUDALib/lib";

		PublicIncludePaths.Add(Path.Combine(poject_root_path, custom_cuda_lib_include));
		PublicAdditionalLibraries.Add(Path.Combine(poject_root_path, custom_cuda_lib_lib, "CUDA_Volume_Rendering.lib"));

		string cuda_path = "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v11.5";
		string cuda_include = "include";
		string cuda_lib = "lib/x64";

		PublicIncludePaths.Add(Path.Combine(cuda_path, cuda_include));

		//PublicAdditionalLibraries.Add(Path.Combine(cuda_path, cuda_lib, "cudart.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(cuda_path, cuda_lib, "cudart_static.lib"));

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
