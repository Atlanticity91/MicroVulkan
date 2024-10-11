#!/bin/bash

pushd ..

echo === Micro Vulkan Setup ===

MicroVulkan/Build/Premake/Linux/premake5 --cc=clang --file=MicroVulkan/Build/Build.lua gmake2

popd
