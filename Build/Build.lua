include "Premake/CSExtensions.lua"
include "Premake/VSExtensions.lua"
include "Build-Dependencies.lua"

workspace "Micro Solution"
    architecture "x64"
    startproject "MicroCore"
    location "../"
    configurations { 
        "Debug", 
        "Release", 
        "Dist" 
    }

    flags "MultiProcessorCompile"

    -- Workspace-wide build options for MSVC
    filter "system:windows"
        buildoptions { 
            "/EHsc",
            "/Zc:preprocessor", 
            "/Zc:__cplusplus" 
        }

    --- Thirdparty Projects
    group "Thirdparty"
        include "Thirdparty/Build-Header-Vulkan.lua"
        include "Thirdparty/Build-Header-Shaderc.lua"
        include "Thirdparty/Build-Header-Spirv.lua"
    group "Test"
    group ""

    --- Main Project
    include "Build-MicroVulkan.lua"
