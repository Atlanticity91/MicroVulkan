@echo off

set scriptpath=%~dp0

pushd ..
cls

echo === Micro Vulkan Setup ===

IF EXIST "MicroVulkan\Micro Solution.sln" del /f "MicroVulkan\Micro Solution.sln"
IF EXIST MicroVulkan\MicroVulkan.vcxproj del /f MicroVulkan\MicroVulkan.vcxproj
IF EXIST MicroVulkan\MicroVulkan.vcxproj.filters del /f MicroVulkan\MicroVulkan.vcxproj.filters
IF EXIST MicroVulkan\MicroVulkan.vcxproj.user del /f MicroVulkan\MicroVulkan.vcxproj.user
IF EXIST MicroVulkan\bin rmdir /q /s MicroVulkan\bin
IF EXIST MicroVulkan\bin-int rmdir /q /s MicroVulkan\bin-int
IF EXIST MicroVulkan\Solution\ rmdir /q /s MicroVulkan\Solution\

MicroVulkan\Build\Premake\Windows\premake5.exe --file=MicroVulkan\Build\Build.lua vs2022

popd

pause
