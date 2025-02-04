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
MicroVulkanSynchronization::MicroVulkanSynchronization( ) 
	: m_syncs{ }
{ }

bool MicroVulkanSynchronization::Create(
	const MicroVulkanDevice& device,
	const MicroVulkanSwapchain& swapchain
) {
	auto& swapchain_spec = swapchain.GetSpecification( );
	auto sync_count		 = swapchain_spec.ImageCount;
	auto state			 = true;

	m_syncs.resize( sync_count );

	while ( state && sync_count-- > 0 ) {
		auto& sync = m_syncs[ sync_count ];

		state = CreateSemaphore( device, sync.Renderable  ) &&
				CreateSemaphore( device, sync.Presentable ) &&
				CreateSignal( device, sync.Signal );
	}

	return state;
}

MicroVulkanSync* MicroVulkanSynchronization::Acquire( const uint32_t frame_id ) {
	micro_assert( frame_id < m_syncs.size( ), "Frame Index must be in range [ 0 : %u ]", m_syncs.size( ) );

	return micro_ptr( m_syncs[ frame_id ] );
}

void MicroVulkanSynchronization::Destroy( const MicroVulkanDevice& device ) {
	for ( auto& sync : m_syncs ) {
		vk::DestroySemaphore( device, sync.Renderable );
		vk::DestroySemaphore( device, sync.Presentable );
		vk::DestroyFence( device, sync.Signal );
	}
}

bool MicroVulkanSynchronization::CreateSemaphore(
	const MicroVulkanDevice& device,
	VkSemaphore& semaphore
) {
	auto specificaton = VkSemaphoreCreateInfo{ };

	specificaton.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	specificaton.pNext = VK_NULL_HANDLE;
	specificaton.flags = VK_UNUSED_FLAG;

	return vk::CreateSemaphore( device, specificaton, semaphore ) == VK_SUCCESS;
}

bool MicroVulkanSynchronization::CreateSignal( 
	const MicroVulkanDevice& device, 
	VkFence& signal 
) {
	auto specification = VkFenceCreateInfo{ };

	specification.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	specification.pNext = VK_NULL_HANDLE;
	specification.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	return vk::CreateFence( device, specification, signal ) == VK_SUCCESS;
}
