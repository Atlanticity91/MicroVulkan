project "Vulkan"
	kind "Utility"
	language "C++"
	location "%{OutputDirs.Solution}"

	--- GLOBAL INCLUDES
	includedirs "%{IncludeDirs.Vulkan}"
	
	externalincludedirs "%{IncludeDirs.Vulkan}"

	--- GLOBAL SOURCE FILES
	files { 
		"%{IncludeDirs.Vulkan}Include/vulkan/**.h",
		"%{IncludeDirs.Vulkan}Include/vulkan/**.hpp"
	}

	--- LINUX
	filter "system:linux"
		--- DEFINES
		defines { "LINUX" }

	--- WINDOWS
	filter "system:windows"
		cppdialect "C++20"

		--- DEFINES
		defines { 
			"WINDOWS",
			"VK_USE_PLATFORM_WIN32_KHR" 
		}
