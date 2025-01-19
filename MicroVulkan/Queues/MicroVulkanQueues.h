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

#include "MicroVulkanDevice.h"

micro_struct MicroVulkanQueue {

	VkBool32 InUse = VK_FALSE;
	VkQueue Queue  = VK_NULL_HANDLE;

	operator const VkQueue& ( ) const {
		return Queue;
	};

};

micro_struct MicroVulkanQueueHandle {

	vk::QueueTypes Type = vk::QUEUE_TYPE_COUNT;
	uint32_t QueueID	= UINT32_MAX;
	VkQueue Queue		= VK_NULL_HANDLE;

	bool GetIsValid( ) const {
		return QueueID < UINT32_MAX && vk::IsValid( Queue );
	};

	operator bool ( ) const {
		return GetIsValid( );
	};

	operator VkQueue ( ) const {
		return Queue;
	};

};

micro_class MicroVulkanQueues final { 

	using QueueList = std::vector<MicroVulkanQueue>;

private:
	mutable std::map<vk::QueueTypes, QueueList> m_queues;
	mutable std::map<vk::QueueTypes, uint32_t> m_queue_families;

public:
	MicroVulkanQueues( );

	~MicroVulkanQueues( ) = default;

	void Create( const MicroVulkanDevice& device );

	MicroVulkanQueueHandle Acquire( vk::QueueTypes type );

	void Release( MicroVulkanQueueHandle& handle );

	void Destroy( );

private:
	QueueList CreateQueues(
		const MicroVulkanDevice& device,
		const vk::QueueTypes type,
		const uint32_t family,
		uint32_t count
	);

public:
	const MicroVulkanQueue& Get( const vk::QueueTypes type ) const;

	uint32_t GetQueueSize( vk::QueueTypes type ) const;

	uint32_t GetQueueFamily( vk::QueueTypes type ) const;

};
