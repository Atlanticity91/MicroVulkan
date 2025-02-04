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

#pragma once

#include "MicroVulkanBufferSpecification.h"

micro_class MicroVulkanBuffer { 

private:
	VkBuffer m_buffer;
	VkDeviceMemory m_memory;

public:
	MicroVulkanBuffer( );

	~MicroVulkanBuffer( ) = default;

	bool Create(
		const MicroVulkanDevice& device,
		const MicroVulkanQueues& queues,
		const MicroVulkanBufferSpecification& specification
	);

	void Destroy( const MicroVulkanDevice& device );

private:
	bool CreateBuffer( 
		const MicroVulkanDevice& device,
		const MicroVulkanQueues& queues,
		const MicroVulkanBufferSpecification& specification
	);

	bool CreateStorage( 
		const MicroVulkanDevice& device,
		const MicroVulkanBufferSpecification& specification
	);

public:
	const VkBuffer& Get( ) const;

private:
	void GetQueueSharingPolicy(
		const MicroVulkanQueues& queues,
		VkBufferCreateInfo& specification
	);

	VkMemoryPropertyFlags GetMemoryProperties(
		const MicroVulkanBufferSpecification& specification
	) const;

public:
	operator const VkBuffer& ( ) const;

};
