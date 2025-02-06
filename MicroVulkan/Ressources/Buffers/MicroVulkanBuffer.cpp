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
MicroVulkanBuffer::MicroVulkanBuffer( ) 
	: m_buffer{ VK_NULL_HANDLE },
	m_memory{ VK_NULL_HANDLE }
{ }

bool MicroVulkanBuffer::Create( 
	const MicroVulkanDevice& device,
	const MicroVulkanQueues& queues,
	const MicroVulkanBufferSpecification& specification
) { 
	return  CreateBuffer( device, queues, specification ) &&
			CreateStorage( device, specification );
}

bool MicroVulkanBuffer::CreateBuffer( 
	const MicroVulkanDevice& device,
	const MicroVulkanQueues& queues,
	const MicroVulkanBufferSpecification& specification
) {
	auto buffer_spec = VkBufferCreateInfo{ };

	buffer_spec.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	buffer_spec.pNext = VK_NULL_HANDLE;
	buffer_spec.flags = VK_UNUSED_FLAG;
	buffer_spec.size  = specification.Capacity;
	buffer_spec.usage = specification.Usage;

	GetQueueSharingPolicy( queues, buffer_spec );

	return vk::CreateBuffer( device, buffer_spec, m_buffer ) == VK_SUCCESS;
}

bool MicroVulkanBuffer::CreateStorage(
	const MicroVulkanDevice& device,
	const MicroVulkanBufferSpecification& specification
) {
	auto properties = GetMemoryProperties( specification );
	auto alloc_spec = device.CreateBufferAllocationSpec( m_buffer, properties );

	return  vk::AllocateMemory( device, alloc_spec, m_memory ) == VK_SUCCESS &&
			vkBindBufferMemory( device, m_buffer, m_memory, 0 ) == VK_SUCCESS;
}

void MicroVulkanBuffer::Destroy( const MicroVulkanDevice& device ) {
	vk::DeallocateMemory( device, m_memory );
	vk::DestroyBuffer( device, m_buffer );
}

const VkBuffer& MicroVulkanBuffer::Get( ) const {
	return m_buffer;
}

void MicroVulkanBuffer::GetQueueSharingPolicy(
	const MicroVulkanQueues& queues,
	VkBufferCreateInfo& specification
) {
	specification.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	specification.queueFamilyIndexCount = 0;
	specification.pQueueFamilyIndices = VK_NULL_HANDLE;

	/*
	* TODO if graphics and present queue are not equal
	specification.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
	specification.queueFamilyIndexCount = 2;
	specification.pQueueFamilyIndices = queueFamilyIndices;
	*/
}

VkMemoryPropertyFlags MicroVulkanBuffer::GetMemoryProperties(
	const MicroVulkanBufferSpecification& specification
) const {
	auto properties = (VkMemoryPropertyFlags)VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

	if ( specification.Usage & VK_BUFFER_USAGE_TRANSFER_SRC_BIT )
		properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

	return properties;
}

MicroVulkanBuffer::operator const VkBuffer& ( ) const {
	return Get( );
}
