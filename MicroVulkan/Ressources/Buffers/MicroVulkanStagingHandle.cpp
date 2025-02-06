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
MicroVulkanStagingHandle::MicroVulkanStagingHandle( ) 
	: StagingID{ 0 },
	Occupancy{ 0 },
	Mapping{ nullptr },
	Handle{ nullptr }
{ }

VkResult MicroVulkanStagingHandle::Copy( const uint32_t length, const void* buffer ) { 
	auto result = VK_SUCCESS;


	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////
//		===	PUBLIC GET ===
////////////////////////////////////////////////////////////////////////////////////////////
bool MicroVulkanStagingHandle::GetIsValid( ) const {
	return Handle != nullptr && Mapping != nullptr;
}

bool MicroVulkanStagingHandle::GetIsFull( )const {
	return Occupancy == Handle->Length;
}

uint32_t MicroVulkanStagingHandle::GetRemaining( ) const {
	return Handle->Length - Occupancy;
}

bool MicroVulkanStagingHandle::GetCanStore( const uint32_t length ) const {
	return Occupancy < Handle->Length && length <= Handle->Length - Occupancy;
}

////////////////////////////////////////////////////////////////////////////////////////////
//		===	OPERATOR ===
////////////////////////////////////////////////////////////////////////////////////////////
MicroVulkanStagingHandle::operator bool ( ) const {
	return GetIsValid( );
}
