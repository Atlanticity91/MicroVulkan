/**
 *
 *  __  __ _          __   __    _ _
 * |  \/  (_)__ _ _ __\ \ / /  _| | |____ _ _ _
 * | |\/| | / _| '_/ _ \ V / || | | / / _` | ' \
 * |_|  |_|_\__|_| \___/\_/ \_,_|_|_\_\__,_|_||_|
 *
 * MIT License
 *
 * Copyright (c) 2024- Alves Quentin
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 **/

#pragma once

#include "../MicroVulkan.h"

micro_struct MicroVulkanCompilerMacro {

	micro_string Name;
	micro_string Value;

};

micro_enum_class MicroVulkanCompilerLanguage : uint32_t {

	GLSL = 0,
	HLSL

};

micro_enum_class MicroVulkanCompilerOptimization : uint32_t {

	Performance = 0,

};

micro_enum_class MicroVulkanCompilerTargetType : uint32_t {

	Assembler = 0,
	Binary

};

micro_enum_class MicroVulkanCompilerEnvironment : uint32_t {

	Vulkan = 0,
	OpenGL

};

micro_struct MicroVulkanCompilerSpecification {

	MicroVulkanCompilerLanguage Language;
	MicroVulkanCompilerOptimization Optimization;
	MicroVulkanCompilerTargetType Target;
	MicroVulkanCompilerEnvironment Environment;
	std::vector<MicroVulkanCompilerMacro> Macros;

};

micro_struct MicroVulkanCompilerResult {

	std::vector<uint8_t> Output;

	MicroVulkanCompilerResult( )
		: Output{ } 
	{ };

	MicroVulkanCompilerResult(
		MicroVulkanCompilerResult&& other
	) noexcept 
		: Output{ std::move( other.Output ) }
	{ };

	bool GetIsValid( ) const {
		return Output.size( ) > 0;
	};

	operator bool ( ) const {
		return GetIsValid( );
	};

};

micro_class MicroVulkanCompiler final {

private:
	shaderc::Compiler m_compiler;
	shaderc::CompileOptions m_options;

public:
	MicroVulkanCompiler( );

	~MicroVulkanCompiler( ) = default;

	void Register( const MicroVulkanCompilerMacro& macro );
	
	void Register( std::initializer_list<MicroVulkanCompilerMacro> macros );
	
	void Register( micro_string name, micro_string value );

	void Register( const std::string& name, const std::string& value );

	MicroVulkanCompilerResult Compile(
		const MicroVulkanCompilerSpecification& specification,
		const std::string source
	);

	MicroVulkanCompilerResult Compile(
		const MicroVulkanCompilerSpecification& specification,
		micro_string code,
		const uint32_t length
	);

	MicroVulkanCompilerResult CompileFile(
		const MicroVulkanCompilerSpecification& specification,
		const std::string path
	);

};
