--- OPTIONS
newoption {
	trigger = "vk_version",
	value = "1.4.304.1",
	description = "Set query minimal Vulkan SDK version"
}

--- ENVIRONEMENT VARIABLES
vulkan = os.getenv( "VULKAN_PATH" )

--- OUTPUT DIRS
OutputDirs = { }

OutputDirs[ "Solution" ] = "%{wks.location}/Solution/"
OutputDirs[ "Bin" ] = "%{wks.location}/bin/"
OutputDirs[ "BinInt" ] = "%{wks.location}/bin-int/"

--- INCLUDE DIRS
IncludeDirs = { }

IncludeDirs[ "Vulkan" ] = vulkan.."/".._OPTIONS[ "vk_version" ].."/"
IncludeDirs[ "MicroVulkan"] = "%{wks.location}/MicroVulkan/"
