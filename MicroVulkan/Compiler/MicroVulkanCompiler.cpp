/** 
 * 
 *  __  __ _             ___             
 * |  \/  (_)__ _ _ ___ / __|___ _ _ ___ 
 * | |\/| | / _| '_/ _ \ (__/ _ \ '_/ -_)
 * |_|  |_|_\__|_| \___/\___\___/_| \___|
 *                                      
 * MIT License
 *
 * Copyright (c) 2024 Alves Quentin
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

#include <__micro_vulkan_pch.h>

////////////////////////////////////////////////////////////////////////////////////////////
//		===	PUBLIC ===
////////////////////////////////////////////////////////////////////////////////////////////
MicroVulkanCompiler::MicroVulkanCompiler( )
{ }

bool MicroVulkanCompiler::Compile( const std::vector<char>& source ) {
	auto* source_ = source.data( );
	auto length   = (uint32_t)source.size( );

	return Compile( length, source_ );
}

bool MicroVulkanCompiler::Compile( const std::string& source ) {
	auto length = (uint32_t)source.length( );

	return Compile( length, source );
}

bool MicroVulkanCompiler::Compile( const uint32_t length, micro_string source ) {
	return false;
}
