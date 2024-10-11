@echo off

set scriptpath=%~dp0

pushd ..
cls

echo === Micro Vulkan Setup ===

IF EXIST MicroVulkan\MicroVulkan.sln del /f MicroVulkan\MicroVulkan.sln
IF EXIST MicroVulkan\bin rmdir /q /s MicroVulkan\bin
IF EXIST MicroVulkan\bin-int rmdir /q /s MicroVulkan\bin-int

MicroVulkan\Build\Premake\Windows\premake5.exe --file=MicroVulkan\Build\Build.lua vs2022

popd

pause
