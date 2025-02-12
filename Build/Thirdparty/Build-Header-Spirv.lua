project "Spirv"
	kind "Utility"
	language "C"
	location "%{OutputDirs.Solution}"

	--- GLOBAL INCLUDES
	includedirs "%{IncludeDirs.vulkan}"

	externalincludedirs "%{IncludeDirs.Vulkan}"

	--- GLOBAL SOURCE FILES
	files { 
		"%{IncludeDirs.Vulkan}/spirv-headers/**.h",
		"%{IncludeDirs.Vulkan}/spirv-headers/**.hpp"
	}

	--- WINDOWS
	filter "system:windows"
		cdialect "C17"
