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

#include "MicroVulkanInstance.h"

micro_class MicroVulkanDevice final {

private:
	vk::DeviceSpecification m_specification;
	VkPhysicalDevice m_physical;
	VkDevice m_device;
	VkPhysicalDeviceMemoryProperties m_memory;

public:
	MicroVulkanDevice( );

	~MicroVulkanDevice( ) = default;

	bool Create( 
		const MicroVulkanInstance& instance,
		const MicroVulkanSpecification& specification
	);

	VkMemoryAllocateInfo CreateImageAllocationSpec( 
		const VkImage& image,
		const VkMemoryPropertyFlags properties
	) const;

	VkMemoryAllocateInfo CreateBufferAllocationSpec( 
		const VkBuffer& buffer,
		const VkMemoryPropertyFlags properties
	) const;

	void Wait( );

	void Destroy( );

private:
	void PreSelectPhysical( 
		const uint32_t api_version, 
		std::vector<VkPhysicalDevice>& physical_list 
	);

	uint32_t CreatePhysicalScore( 
		const MicroVulkanSpecification& specification,
		const VkSurfaceKHR& surface,
		const VkPhysicalDevice& physical 
	);

	void SelectPhysical( 
		const MicroVulkanSpecification& specification,
		const MicroVulkanInstance& instance,
		std::vector<VkPhysicalDevice>& physical_list 
	);

	bool CreatePhysical( 
		const MicroVulkanSpecification& specification,
		const MicroVulkanInstance& instance 
	);

	std::vector<VkDeviceQueueCreateInfo> CreatePhysicalQueues( const std::vector<float>& priorities );

	bool CreateDevice( const MicroVulkanSpecification& specification );

public:
	const vk::DeviceSpecification& GetSpecification( ) const;

	VkPhysicalDevice GetPhysical( ) const;

	VkDevice GetDevice( ) const;

	uint32_t GetPeekMemoryType( 
		const VkMemoryPropertyFlags properties,
		uint32_t requirement_bits
	) const;

private:
	bool GetPhysicalHasExtensions(
		const MicroVulkanSpecification& specification,
		const VkPhysicalDevice& physical
	);

	bool GetPhysicalHasFeatures(
		const MicroVulkanSpecification& specification,
		const vk::DeviceSpecification& physical_spec
	);

	bool GetPhysicalHasDepth(
		const MicroVulkanSpecification& specification,
		const VkPhysicalDevice& physical
	);

	uint32_t GetPhysicalQueueScore( const vk::DeviceSpecification& physical_spec );

public:
	operator VkDevice ( ) const;

};
