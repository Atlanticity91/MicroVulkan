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

#include "__micro_vulkan_pch.h"

////////////////////////////////////////////////////////////////////////////////////////////
//		===	PUBLIC ===
////////////////////////////////////////////////////////////////////////////////////////////
MicroVulkanCompiler::MicroVulkanCompiler( )
	: m_compiler{ },
	m_options{ }
{ }

void MicroVulkanCompiler::Register( const MicroVulkanCompilerMacro& macro ) {
	Register( macro.Name, macro.Value );
}

void MicroVulkanCompiler::Register( 
	std::initializer_list<MicroVulkanCompilerMacro> macros 
) {
	for ( const auto& macro : macros )
		Register( macro.Name, macro.Value );
}

void MicroVulkanCompiler::Register( micro_string name, micro_string value ) {
	const auto name_length  = strlen( name );
	const auto value_length = strlen( value );

	if ( name_length == 0 || value_length == 0 )
		return;

	m_options.AddMacroDefinition( name, name_length, value, value_length );
}

void MicroVulkanCompiler::Register( const std::string& name, const std::string& value ) {
	const auto* name_string  = name.c_str( );
	const auto* value_string = value.c_str( );

	Register( name_string, value_string );
}

MicroVulkanCompilerResult MicroVulkanCompiler::Compile(
	const MicroVulkanCompilerSpecification& specification,
	const std::string source
) {
	const auto* code  = source.c_str( );
	const auto length = (uint32_t)source.size( );

	return Compile( specification, code, length );
}

MicroVulkanCompilerResult MicroVulkanCompiler::Compile(
	const MicroVulkanCompilerSpecification& specification,
	micro_string code,
	const uint32_t length
) {
	auto result = MicroVulkanCompilerResult{ };

	return result;
}

MicroVulkanCompilerResult MicroVulkanCompiler::CompileFile(
	const MicroVulkanCompilerSpecification& specification,
	const std::string path
) {
	auto result = MicroVulkanCompilerResult{ };

	return result;
}

/*
bool ProcessGLSL( const MicroVulkanCompilerSource& source, shaderc::CompileOptions& options  ) {
	auto* source_str = context.Source.get( );
	auto source_len  = context.Source.length( );
	auto* name_str   = context.Name.get( );
	auto preprocess  = m_compiler.PreprocessGlsl( source_str, source_len, shaderc_glsl_infer_from_source, name_str, options );
	auto status		 = preprocess.GetCompilationStatus( );
	
	if ( status == shaderc_compilation_status_success ) {
		auto spirv = m_compiler.CompileGlslToSpv( preprocess.begin( ), shaderc_glsl_infer_from_source, name_str, options );

		status = spirv.GetCompilationStatus( );
		
		if ( status == shaderc_compilation_status_success ) {
			auto size = tiny_cast( spirv.end( ) - spirv.begin( ), tiny_uint ) * tiny_sizeof( tiny_uint );
			
			specification.Type  = PeekType( context.Source );
			specification.Entry = context.Entry.as_string( );
			specification.Code  = size;

			auto* src = spirv.begin( );
			auto* dst = specification.Code.data( );

			Tiny::Memcpy( src, dst, size );
		} else
			printf( "[ VK ] Shader Compilation Error : %s\n%s\n", name_str, spirv.GetErrorMessage( ).c_str( ) );
	} else 
		printf( "[ VK ] Shader Compilation Error : %s\n%s\n", name_str, preprocess.GetErrorMessage( ).c_str( ) );

	return status == shaderc_compilation_status_success;
}

bool ProcessHLSL( const MicroVulkanCompilerSource& source, shaderc::CompileOptions& options ) {

	return false;
}

bool d( const MicroVulkanCompilerSource& source ) {
	// OPTIONS
	auto options = shaderc::CompileOptions{ m_options };
	auto result  = false;

	options.SetSourceLanguage( shaderc_source_language_glsl ); // source.Language
	options.SetOptimizationLevel( shaderc_optimization_level_performance ); // source.Optimization
	options.SetTargetEnvironment( shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_3 ); //source.Environment

	for ( const auto& macro : source.Macros ) {
		const auto name_length  = strlen( macro.Name );
		const auto value_length = strlen( macro.Value );

		options.AddMacroDefinition( macro.Name, name_length, macro.Value, value_length );	
	}

	// COMPILATION
	switch ( source.Language ) {
		case MicroVulkanCompilerLanguage::GLSL : result = ProcessGLSL( source, options ); break;
		case MicroVulkanCompilerLanguage::HLSL : result = ProcessHLSL( source, options ); break;

		default : break;
	}

	return result;
}
*/
