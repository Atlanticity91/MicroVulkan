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
MicroVulkanStagings::MicroVulkanStagings( )
	: m_stagings{ }
{ }

bool MicroVulkanStagings::Create( 
	const MicroVulkanDevice& device
) {
	return true;
}

MicroVulkanStagingHandle MicroVulkanStagings::Acquire(
	const MicroVulkanDevice& device,
	const MicroVulkanQueueHandle& queue
) { 
	auto handle = MicroVulkanStagingHandle{ };

	//vkMapMemory( device, m_stagings[ handle.StagingID ].Memory, 0, VK_WHOLE_SIZE, VK_UNUSED_FLAG, (void**)&handle.Mapping );

	return handle;
}

void MicroVulkanStagings::Release( 
	const MicroVulkanDevice& device, 
	MicroVulkanStagingHandle& handle 
) {
	if ( handle.GetIsValid( ) ) {
		auto& staging = m_stagings[ handle.StagingID ];

		staging.InUse = VK_FALSE;

		//vkUnmapMemory( device, staging.Memory );

		handle.StagingID = 0;
		handle.Occupancy = 0;
		handle.Mapping   = nullptr;
		handle.Handle	 = nullptr;
	}
}

void MicroVulkanStagings::Destroy( const MicroVulkanDevice& device ) {
}

bool MicroVulkanStagings::CreateStagingBuffer(
	const MicroVulkanDevice& device,
	const MicroVulkanQueues& queues
) {
	auto queue_count = queues.GetQueueSize( vk::QUEUE_TYPE_TRANSFERT );

	m_stagings.resize( queue_count );

	while ( queue_count-- > 0 ) {

	}

	return true;
}
