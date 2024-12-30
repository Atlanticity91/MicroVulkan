--- TOOL INCLUDES
include "Premake/CSExtensions.lua"
include "Premake/VSExtensions.lua"
include "Build-Dependencies.lua"

--- PROJECT CONFIGURATION
workspace "Micro Solution"
    architecture "x64"
    startproject "MicroCore"
    location "../"
    configurations { 
        "Debug", 
        "Release", 
        "Dist" 
    }

    --- GLOBAL FLAGS
    flags "MultiProcessorCompile"

    --- WINDOWS
    filter "system:windows"
        --- WORKSPACE-WIDE BUILD OPTIONS FOR MSVC
        buildoptions { 
            "/EHsc",
            "/Zc:preprocessor", 
            "/Zc:__cplusplus" 
        }

    --- THIRDPARTY PROJECTS
    group "Thirdparty"
        include "Thirdparty/Build-Header-Vulkan.lua"
        include "Thirdparty/Build-Header-Shaderc.lua"
        include "Thirdparty/Build-Header-Spirv.lua"
    --- TEST PROJECTS
    group "Test"
    group ""

    --- MAIN PROJECT
    include "Build-MicroVulkan.lua"
