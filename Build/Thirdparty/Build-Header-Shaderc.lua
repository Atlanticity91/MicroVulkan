project "Shaderc"
	kind "Utility"
	language "C"
	location "%{OutputDirs.Solution}"

	--- GLOBAL INCLUDES
	includedirs "%{IncludeDirs.Vulkan}"
	
	externalincludedirs "%{IncludeDirs.vulkan}"

	--- GLOBAL SOURCE FILES
	files { 
		"%{IncludeDirs.Vulkan}Include/shaderc/**.h",
		"%{IncludeDirs.Vulkan}Include/shaderc/**.hpp"
	}

	--- WINDOWS
	filter "system:windows"
		cdialect "C17"
