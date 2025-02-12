project "Shaderc"
	kind "Utility"
	language "C"
	location "%{OutputDirs.Solution}"

	--- GLOBAL INCLUDES
	includedirs "%{IncludeDirs.Vulkan}"
	
	externalincludedirs "%{IncludeDirs.vulkan}"

	--- GLOBAL SOURCE FILES
	files { 
		"%{IncludeDirs.Vulkan}/shaderc/**.h",
		"%{IncludeDirs.Vulkan}/shaderc/**.hpp"
	}

	--- WINDOWS
	filter "system:windows"
		cdialect "C17"
