project "MicroVulkan"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

	defines { "_CRT_SECURE_NO_WARNINGS" }

	files { "../MicroVulkan/**.h", "../MicroVulkan/**.cpp" }

	pchheader "__micro_vulkan_pch.h"
	pchsource "../MicroVulkan/__micro_vulkan_pch.cpp"

	targetdir "%{wks.location}/bin/"
	objdir "%{wks.location}/bin-int/%{prj.name}"

	vulkan = os.getenv( "VULKAN_PATH" )

	includedirs { 
		"%{wks.location}/MicroVulkan/",
        vulkan.."/Include/" 
    }
	externalincludedirs { 
		"%{wks.location}/MicroVulkan/",
        vulkan.."/Include/" 
	}

	filter "system:windows"
		systemversion "latest"
		defines { "WINDOWS" }
		flags { "MultiProcessorCompile" }

	filter "configurations:Debug"
		defines { "DEBUG" }
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines { "RELEASE" }
		runtime "Release"
		optimize "On"
		symbols "On"

	filter "configurations:Dist"
		defines { "DIST" }
		runtime "Release"
		optimize "On"
		symbols "Off"
