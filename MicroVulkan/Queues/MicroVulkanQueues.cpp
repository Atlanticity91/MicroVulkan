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
MicroVulkanQueues::MicroVulkanQueues( )
	: m_queues{ },
	m_queue_families{ }
{ }

void MicroVulkanQueues::Create( const MicroVulkanDevice& device ) {
	auto& device_spec = device.GetSpecification( );
	auto queue_idx    = vk::QUEUE_TYPE_GRAPHICS;
	auto* queues	  = micro_cast( device_spec.Queues, const uint32_t* );

	while ( queue_idx < vk::QUEUE_TYPE_COUNT ) {
		auto queue_id   = 2 * (uint32_t)queue_idx;
		auto queue_list = CreateQueues( device, queue_idx, queues[ queue_id ], queues[ queue_id + 1 ] );

		m_queues.emplace( queue_idx, queue_list );
		m_queue_families.emplace( queue_idx, queues[ queue_id ] );

		queue_idx = (vk::QueueTypes)( queue_idx + 1 );
	}
}

MicroVulkanQueueHandle MicroVulkanQueues::Acquire( vk::QueueTypes type ) {
	auto& queue_list = m_queues[ type ];
	auto queue_id    = (uint32_t)0;
	auto handle		 = MicroVulkanQueueHandle{ };

	for ( auto& queue : queue_list ) {
		if ( queue.InUse == VK_TRUE )
			queue_id += 1;
		else {
			queue.InUse  = VK_TRUE;

			handle.Type	   = type;
			handle.QueueID = queue_id;
			handle.Queue   = queue.Queue;

			break;
		}
	}

	return handle;
}

void MicroVulkanQueues::Release( MicroVulkanQueueHandle& handle ) {
	if ( handle.GetIsValid( ) ) {
		auto& queue_list = m_queues[ handle.Type ];

		queue_list[ handle.QueueID ].InUse = VK_FALSE;

		handle.QueueID = UINT32_MAX;
		handle.Queue   = VK_NULL_HANDLE;
	}
}

void MicroVulkanQueues::Destroy( ) {
}

////////////////////////////////////////////////////////////////////////////////////////////
//		===	PRIVATE ===
////////////////////////////////////////////////////////////////////////////////////////////
MicroVulkanQueues::QueueList MicroVulkanQueues::CreateQueues(
	const MicroVulkanDevice& device,
	const vk::QueueTypes type,
	const uint32_t family,
	uint32_t count
) {
	auto queues = QueueList( count );

	while ( count-- > 0 ) {
		queues[ count ].InUse = VK_FALSE;

		vkGetDeviceQueue( device, family, count, micro_ptr( queues[ count ].Queue ) );
	}

	return queues;
}

////////////////////////////////////////////////////////////////////////////////////////////
//		===	PUBLIC GET ===
////////////////////////////////////////////////////////////////////////////////////////////
const MicroVulkanQueue& MicroVulkanQueues::Get( const vk::QueueTypes type ) const {
	return m_queues[ type ][ 0 ];
}

uint32_t MicroVulkanQueues::GetQueueSize( vk::QueueTypes type ) const {
	return (uint32_t)m_queues[ type ].size( );
}

uint32_t MicroVulkanQueues::GetQueueFamily( vk::QueueTypes type ) const {
	return m_queue_families[ type ];
}
