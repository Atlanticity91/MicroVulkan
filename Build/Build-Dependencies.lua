--- OUTPUT DIRS
OutputDirs = { }

OutputDirs[ "Solution" ] = "%{wks.location}/Solution/"
OutputDirs[ "Bin" ] = "%{wks.location}/bin/"
OutputDirs[ "BinInt" ] = "%{wks.location}/bin-int/"

--- ENVIRONEMENT VARIABLES
vulkan = os.getenv( "VULKAN_PATH" )

--- THIRDPARTY LIBRARIES
IncludeDirs = { }

IncludeDirs[ "Vulkan" ] = vulkan.."/Include/"
IncludeDirs[ "MicroVulkan"] = "%{wks.location}/MicroVulkan/"
