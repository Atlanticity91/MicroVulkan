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

#include "../Ressources/MicroVulkanStagings.h"

struct MicroVulkanCommandBuffer {

	VkBool32 InUse			   = VK_FALSE;
	VkCommandBufferLevel Level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	VkCommandBuffer Buffer	   = VK_NULL_HANDLE;

	operator VkCommandBuffer ( ) {
		return Buffer;
	};

};

struct MicroVulkanCommandPoolSpecification {

	uint32_t QueueFamily    = 0;
	uint32_t PrimayCount    = 0;
	uint32_t SecondaryCount = 0;

};

struct MicroVulkanCommandPool {

	VkCommandPool Pool = VK_NULL_HANDLE;
	std::vector<MicroVulkanCommandBuffer> Buffers{ };

	operator VkCommandPool& ( ) {
		return Pool;
	};

};

struct MicroVulkanCommandHandle {

	vk::QueueTypes Type			   = vk::QUEUE_TYPE_COUNT;
	uint32_t BufferID			   = UINT32_MAX;
	VkCommandBufferLevel Level	   = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	mutable VkCommandBuffer Buffer = VK_NULL_HANDLE;

	bool GetIsValid( ) const {
		return BufferID < UINT32_MAX && vk::IsValid( Buffer );
	};

	operator bool ( ) const {
		return GetIsValid( );
	};

	operator VkCommandBuffer& ( ) const {
		return Buffer;
	};

};

micro_class MicroVulkanCommands final { 

private:
	std::map<vk::QueueTypes, MicroVulkanCommandPool> m_pools;

public:
	MicroVulkanCommands( );

	~MicroVulkanCommands( ) = default;

	bool Create( 
		const MicroVulkanDevice& device,
		const MicroVulkanQueues& queues,
		const MicroVulkanSwapchain& swapchain
	);

	MicroVulkanCommandHandle Acquire( vk::QueueTypes queue_type, VkCommandBufferLevel level );

	void Release( MicroVulkanCommandHandle& handle );

	void Destroy( const MicroVulkanDevice& device );

private:
	MicroVulkanCommandPoolSpecification CreateCommandPoolSpec(
		const MicroVulkanQueues& queues,
		const MicroVulkanSwapchain& swapchain,
		vk::QueueTypes queue_type
	);

	bool CreateCommandPool(
		const MicroVulkanDevice& device,
		const MicroVulkanCommandPoolSpecification& pool_spec,
		MicroVulkanCommandPool& pool
	);

	VkCommandBufferAllocateInfo CreateCommandBuffersSpec(
		const MicroVulkanCommandPool& pool,
		const VkCommandBufferLevel level,
		const uint32_t count
	);

	void CreateCommandBufferList( 
		const VkCommandBufferLevel level,
		const std::vector<VkCommandBuffer>& buffer_list,
		const uint32_t offset,
		MicroVulkanCommandPool& pool
	);

	bool CreateCommandBuffers(
		const MicroVulkanDevice& device,
		const VkCommandBufferLevel level,
		const uint32_t offset,
		const uint32_t count,
		MicroVulkanCommandPool& pool
	);

private:
	std::vector<VkCommandBuffer> EnumerateCommandBuffer( 
		const MicroVulkanCommandPool& pool 
	);

};
