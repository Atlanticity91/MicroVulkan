workspace "MicroVulkan"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "MicroVulkan"
   location "../"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

group "Micro"
   include "Build-MicroVulkan.lua"
group ""
