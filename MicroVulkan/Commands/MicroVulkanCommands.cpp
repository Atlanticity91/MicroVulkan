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
MicroVulkanCommands::MicroVulkanCommands( ) 
	: m_pools{ }
{ }

bool MicroVulkanCommands::Create(
	const MicroVulkanDevice& device,
	const MicroVulkanQueues& queues,
	const MicroVulkanSwapchain& swapchain
) {
	auto state = false;
	auto type  = (uint32_t)vk::QUEUE_TYPE_GRAPHICS;

	while ( state && type < vk::QUEUE_TYPE_COUNT ) {
		auto pool_spec = CreateCommandPoolSpec( queues, swapchain, (vk::QueueTypes)type );
		auto pool	   = MicroVulkanCommandPool{ };

		if ( state = CreateCommandPool( device, pool_spec, pool ) ) {
			if ( CreateCommandBuffers( device, VK_COMMAND_BUFFER_LEVEL_PRIMARY, 0, pool_spec.PrimayCount, pool ) ) {
				if ( pool_spec.SecondaryCount > 0 )
					state = CreateCommandBuffers( device, VK_COMMAND_BUFFER_LEVEL_SECONDARY, pool_spec.PrimayCount, pool_spec.SecondaryCount, pool );
			}

			m_pools.emplace( (vk::QueueTypes)type, pool );
		}

		type += 1;
	}

	return state;
}

MicroVulkanCommandHandle MicroVulkanCommands::Acquire(
	vk::QueueTypes queue_type, 
	VkCommandBufferLevel level 
) {
	auto buffer_id = (uint32_t)0;
	auto handle	   = MicroVulkanCommandHandle{ };
	auto& pool	   = m_pools[ queue_type ];

	for ( auto& buffer : pool.Buffers ) {
		if ( buffer.InUse == VK_TRUE || buffer.Level != level )
			buffer_id += 1;
		else {
			buffer.InUse = VK_TRUE;

			handle.Type		= queue_type;
			handle.BufferID = buffer_id;
			handle.Level	= level;
			handle.Buffer	= buffer.Buffer;

			break;
		}
	}

	return handle;
}

void MicroVulkanCommands::Release( MicroVulkanCommandHandle& handle ) {
	if ( handle.GetIsValid( ) ) {
		auto& pool = m_pools[ handle.Type ];

		pool.Buffers[ handle.BufferID ].InUse = VK_FALSE;

		handle.BufferID = UINT32_MAX;
		handle.Buffer   = VK_NULL_HANDLE;
	}
}

void MicroVulkanCommands::Destroy( const MicroVulkanDevice& device ) {
	for ( auto& pair : m_pools ) {
		auto& pool = pair.second;

		if ( vk::IsValid( pool.Pool ) ) {
			auto buffer_list  = EnumerateCommandBuffer( pool );
			auto buffer_count = (uint32_t)buffer_list.size( );
			auto* buffer_data = buffer_list.data( );

			vkFreeCommandBuffers( device, pool, buffer_count, buffer_data );
		}

		vk::DestroyCommandPool( device, pool );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
//		===	PRIVATE ===
////////////////////////////////////////////////////////////////////////////////////////////
MicroVulkanCommandPoolSpecification MicroVulkanCommands::CreateCommandPoolSpec(
	const MicroVulkanQueues& queues,
	const MicroVulkanSwapchain& swapchain,
	vk::QueueTypes queue_type
) {
	auto specification = MicroVulkanCommandPoolSpecification{ };
	auto image_count   = swapchain.GetSpecification( ).ImageCount;
	auto queue_count   = queues.GetQueueSize( queue_type );

	specification.QueueFamily = queues.GetQueueFamily( queue_type );

	if ( queue_type == vk::QUEUE_TYPE_GRAPHICS ) {
		specification.PrimayCount    = image_count;
		specification.SecondaryCount = ( image_count < queue_count ) ? queue_count - image_count : 0;
	} else {
		specification.PrimayCount	 = queue_count;
		specification.SecondaryCount = 0;
	}

	return specification;
}

bool MicroVulkanCommands::CreateCommandPool(
	const MicroVulkanDevice& device,
	const MicroVulkanCommandPoolSpecification& pool_spec,
	MicroVulkanCommandPool& pool
) {
	auto specification = VkCommandPoolCreateInfo{ };

	specification.sType			   = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	specification.pNext			   = VK_NULL_HANDLE;
	specification.flags			   = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	specification.queueFamilyIndex = pool_spec.QueueFamily;

	pool.Buffers.resize( (size_t)( pool_spec.PrimayCount + pool_spec.SecondaryCount ) );

	return vk::CreateCommandPool( device, specification, pool ) == VK_SUCCESS;
}

VkCommandBufferAllocateInfo MicroVulkanCommands::CreateCommandBuffersSpec(
	const MicroVulkanCommandPool& pool,
	const VkCommandBufferLevel level,
	const uint32_t count
) {
	auto specification = VkCommandBufferAllocateInfo{ };
	
	specification.sType				 = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	specification.pNext				 = VK_NULL_HANDLE;
	specification.commandPool		 = pool.Pool;
	specification.level				 = level;
	specification.commandBufferCount = count;

	return specification;
}

void MicroVulkanCommands::CreateCommandBufferList(
	const VkCommandBufferLevel level,
	const std::vector<VkCommandBuffer>& buffer_list,
	const uint32_t offset,
	MicroVulkanCommandPool& pool
) {
	auto buffer_count = buffer_list.size( );

	while ( buffer_count-- > 0 ) {
		auto buffer = MicroVulkanCommandBuffer{ };

		buffer.InUse  = VK_FALSE;
		buffer.Level  = level;
		buffer.Buffer = buffer_list[ buffer_count ];

		pool.Buffers[ buffer_count + offset ] = buffer;
	}
}

bool MicroVulkanCommands::CreateCommandBuffers(
	const MicroVulkanDevice& device,
	const VkCommandBufferLevel level,
	const uint32_t offset,
	const uint32_t count,
	MicroVulkanCommandPool& pool
) {
	auto specification = CreateCommandBuffersSpec( pool, level, count );
	auto buffer_list   = std::vector<VkCommandBuffer>( (size_t)count );
	auto* buffer_data  = buffer_list.data( );
	auto result		   = vkAllocateCommandBuffers( device, micro_ptr( specification ), buffer_data );

	if ( result == VK_SUCCESS )
		CreateCommandBufferList( level, buffer_list, offset, pool );

	return result == VK_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////////////////
//		===	PRIVATE GET ===
////////////////////////////////////////////////////////////////////////////////////////////
std::vector<VkCommandBuffer> MicroVulkanCommands::EnumerateCommandBuffer(
	const MicroVulkanCommandPool& pool
) {
	auto buffer_count = pool.Buffers.size( );
	auto buffer_list  = std::vector<VkCommandBuffer>( (size_t)buffer_count );

	while ( buffer_count-- > 0 )
		buffer_list[ buffer_count ] = pool.Buffers[ buffer_count ].Buffer;

	return buffer_list;
}
