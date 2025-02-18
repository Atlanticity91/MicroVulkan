/**
 *
 *  __  __ _             ___
 * |  \/  (_)__ _ _ ___ / __|___ _ _ ___
 * | |\/| | / _| '_/ _ \ (__/ _ \ '_/ -_)
 * |_|  |_|_\__|_| \___/\___\___/_| \___|
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

extern "C" {
	#include <stddef.h>
	#include <stdlib.h>
	#include <stdio.h>
	#include <string.h>
};

#include <algorithm>
#include <array>
#include <atomic>
#include <bitset>
#include <cassert>
#include <chrono>
#include <concepts>
#include <condition_variable>
#include <filesystem>
#include <format>
#include <functional>
#include <iterator>
#include <limits>
#include <map>
#include <memory>
#include <mutex>
#include <new>
#include <optional>
#include <queue>
#include <random>
#include <regex>
#include <set>
#include <shared_mutex>
#include <string>
#include <thread>
#include <type_traits>
#include <unordered_map>
#include <vector>
#include <xhash>

/**
 * micro_cast macro
 * @note : Cast C style pointer to another pointer type.
 * @param VAR : Query source pointer.
 * @param TYPE : Query destination pointer type.
 **/
#define micro_cast( VAR, TYPE ) ( (TYPE)(VAR) )

/**
 * micro_ptr macro
 * @note : Get the address of a variable.
 * @param VAR : Query source variable.
 **/
#define micro_ptr( VAR ) ( &(VAR) )

/**
 * micro_ptr_as macro
 * @note : Get the address of a variable and cast result as TYPE*.
 * @param VAR : Query source variable.
 * @param TYPE : Query destination pointer type.
 **/
#define micro_ptr_as( VAR, TYPE ) micro_cast( micro_ptr( VAR ), TYPE )

/**
 * micro_ref macro
 * @note : Get reference from a C pointer.
 * @param VAR : s
 **/
#define micro_ref( VAR ) ( *(VAR) )

/**
 * micro_ref_as macro
 * @note : Get reference from a C pointer.
 **/
#define micro_ref_as( VAR, TYPE ) micro_ref( micro_cast( VAR, TYPE* ) )

/**
 * micro_unused macro
 * @note : Mark function parameter unused for the compiler.
 **/
#define micro_unused( VAR ) ( (void)VAR )

#define micro_mask( VAR, MASK ) ( VAR & MASK )

/**
 * micro_compile_if macro
 * @note : Wrapper for Compile time if dispatch.
 **/
#define micro_compile_if( ... ) if constexpr ( __VA_ARGS__ ) 

/**
 * micro_compile_elif macro
 * @note : Wrapper for Compile time else if dispatch.
 **/
#define micro_compile_elif( ... ) else micro_compile_if( __VA_ARGS__ )

/**
 * micro_compile_else macro
 * @note : Wrapper for Compile time else dispatch.
 **/
#define micro_compile_else else

/**
 * micro_depreated macro
 * @note : Mark element deprecated with since message.
 **/
#define micro_depreated( SINCE ) [[deprecated( "Since : " micro_stringify( SINCE ) )]]

/**
 * micro_depreated_for macro
 * @note : Mark element deprecated with since message and replacement indication.
 **/
#define micro_depreated_for( SINCE, REPLACEMENT )\
	[[deprecated( "Since : " micro_stringify( SINCE ) ", instead use : " micro_stringify( REPLACEMENT ) )]]

/**
 * micro_inline macro
 * @note : Wrapper for inline keyword.
 **/
#ifdef _WIN64
#	define micro_inline inline
#else
#	define micro_inline __inline__
#endif

/**
 * micro_nodiscard_cause macro
 * @note : Mark that function return value can't be discarded with cause message.
 * @param MESSAGE : Cause of non discard.
 **/
#define micro_nodiscard_cause( MESSAGE ) [[nodiscard( MESSAGE )]]

/**
 * micro_nodiscard macro
 * @note : Mark that function return value can't be discarded.
 **/
#define micro_nodiscard [[nodiscard]]

/**
 * micro_self macro
 * @note : Wrapper for class member function *this.
 **/
#define micro_self micro_ref( this )

/**
 * MICRO_API macro
 * @note : Defined API for library expose elements.
 **/
#ifndef MICRO_API
#	ifdef MICRO_EXPORT
#		ifdef _WIN64
#			define MICRO_API __declspec( dllexport )
#		endif
#	elif MICRO_IMPORT
#		ifdef _WIN64
#			define MICRO_API __declspec( dllimport )
#		endif
#	endif
#	define MICRO_API
#endif
 
/**
 * micro_enum_class macro
 * @note : Wrapper for exposing enum.
 **/
#define micro_enum_class enum class MICRO_API 

/**
 * micro_struct macro
 * @note : Wrapper for exposing struct.
 **/
#define micro_struct struct MICRO_API

/**
 * micro_interface macro
 * @note : Wrapper for 'interface'.
 **/
#define micro_interface micro_struct

/**
 * micro_class macro
 * @note : Wrapper for exposing class.
 **/
#define micro_class class MICRO_API

/**
 * micro_abstract macro
 * @note : Wrapper for C++ abstract function syntax.
 **/
#define micro_abstract( SIGNATURE ) virtual SIGNATURE = 0

/**
 * micro_implement macro
 * @note : Wrapper for C++ override function syntax, to use in 
 *		   addition of micro_abstract for clarity.
 **/
#define micro_implement( SIGNATURE ) virtual SIGNATURE override

#define micro_optional( SIGNATURE, RETURN ) virtual SIGNATURE { return RETURN; };

#define micro_optional2( SIGNATURE ) micro_optional( SIGNATURE, )

/**
 * micro_stringifyx macro
 * @note : Stringify text.
 * @param TEXT : Query text to stringify.
 **/
#define micro_stringifyx( TEXT ) ( #TEXT )

/**
 * micro_stringify macro
 * @note : Wrapper for stringify to prevent macro substitution error.
 * @param TEXT : Query text to stringify.
 **/
#define micro_stringify( TEXT ) micro_stringifyx( TEXT )

/**
 * micro_sizeof macro
 * @note : Wrapper for sizeof operator.
 * @param TYPE : Query type for sizeof operation.
 **/
#define micro_sizeof( TYPE ) ( (uint32_t)sizeof( TYPE ) )

#define micro_array_size( ARRAY ) ( micro_sizeof( ARRAY ) / micro_sizeof( (ARRAY)[ 0 ] ) )

/**
 * micro_variadic_expand macro
 * @note : Wrapper for template variadic parameter folf expression, simplify reading.
 **/
#define micro_variadic_expand( ... ) ( [&] { __VA_ARGS__ }( ), ... )

/**
 * micro_string typedef
 * @note : Wrapper for C style string.
 **/
typedef const char* micro_string;

/**
 * micro_array_size template
 * @note : Restrict Count to never be 0, 0 size array is illegal.
 * @template Count : Query array size.
 **/
template<uint32_t Count>
constexpr uint32_t micro_array_size = ( Count > 0 ) ? Count : 1;

/**
 * @brief Define a wrapper for std::move support for types.
 **/
template<typename Type>
concept micro_is_movable = std::is_move_constructible_v<Type> && std::is_move_assignable_v<Type>;
