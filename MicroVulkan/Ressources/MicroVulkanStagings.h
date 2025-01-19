/**
 *
 *  __  __ _          __   __    _ _
 * |  \/  (_)__ _ _ __\ \ / /  _| | |____ _ _ _
 * | |\/| | / _| '_/ _ \ V / || | | / / _` | ' \
 * |_|  |_|_\__|_| \___/\_/ \_,_|_|_\_\__,_|_||_|
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

#pragma once

#include "MicroVulkanBuffer.h"

struct MicroVulkanStagingBuffer {

	VkBool32 InUse  = VK_FALSE;
	uint32_t Length = 0;
	VkFence Sync	= VK_NULL_HANDLE;
	MicroVulkanBuffer Buffer{ };

};

struct MicroVulkanStagingHandle {

	uint32_t StagingID				 = 0;
	uint32_t Occupancy				 = 0;
	uint8_t* Mapping				 = nullptr;
	MicroVulkanStagingBuffer* Handle = nullptr;

	VkResult Copy( const uint32_t length, const void* buffer ) { 
		auto result = VK_SUCCESS;


		return result;
	};

	bool GetIsValid( ) const {
		return Handle != nullptr && Mapping != nullptr;
	};

	bool GetIsFull( )const {
		return Occupancy == Handle->Length;
	};

	uint32_t GetRemaining( ) const {
		return Handle->Length - Occupancy;
	};

	bool GetCanStore( const uint32_t length ) const {
		return Occupancy < Handle->Length && length <= Handle->Length - Occupancy;
	};

	operator bool ( ) const {
		return GetIsValid( );
	};

};

class MicroVulkanStagings final {

private:
	std::vector<MicroVulkanStagingBuffer> m_stagings;

public:
	MicroVulkanStagings( );

	~MicroVulkanStagings( ) = default;

	bool Create( 
		const MicroVulkanDevice& device
	);

	MicroVulkanStagingHandle Acquire(
		const MicroVulkanDevice& device,
		const MicroVulkanQueueHandle& queue
	);

	void Release( 
		const MicroVulkanDevice& device, 
		MicroVulkanStagingHandle& handle 
	);

	void Destroy( const MicroVulkanDevice& device );

private:
	bool CreateStagingBuffer(
		const MicroVulkanDevice& device,
		const MicroVulkanQueues& queues
	);

};
