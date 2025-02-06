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

#include "MicroVulkanSwapchainImage.h"

micro_class MicroVulkanSwapchain final {

private:
	MicroVulkanSwapchainSpecification m_specification;
	VkSwapchainKHR m_swapchain;
	std::vector<MicroVulkanSwapchainImage> m_images;

public:
	MicroVulkanSwapchain( );

	~MicroVulkanSwapchain( ) = default;

	bool Create(
		const MicroVulkanWindow& window,
		const MicroVulkanInstance& instance,
		const MicroVulkanDevice& device
	);

	void Recreate(
		const MicroVulkanInstance& instance,
		const MicroVulkanDevice& device,
		const micro_upoint& dimensions
	);

	void Destroy( const MicroVulkanDevice& device );

private:
	void CreateSwapchainSurface( 
		const MicroVulkanInstance& instance,
		const VkSurfaceCapabilitiesKHR& surface_spec,
		VkSwapchainCreateInfoKHR& specification
	);

	void CreateSwapchainFormat( 
		const MicroVulkanInstance& instance,
		const MicroVulkanDevice& device,
		VkSwapchainCreateInfoKHR& specification
	);

	void CreateSwapchainExtent(
		const VkSurfaceCapabilitiesKHR& surface_spec,
		const micro_upoint& dimensions,
		VkSwapchainCreateInfoKHR& specification
	);

	void CreateSwapchainPresent( 
		const MicroVulkanInstance& instance,
		const MicroVulkanDevice& device,
		VkSwapchainCreateInfoKHR& specification
	);

	VkSwapchainCreateInfoKHR CreateSwapchainSpec(
		const MicroVulkanInstance& instance,
		const MicroVulkanDevice& device,
		const micro_upoint& dimensions
	);

	bool CreateSwapchain( 
		const MicroVulkanWindow& window,
		const MicroVulkanInstance& instance,
		const MicroVulkanDevice& device
	);

	bool CreateImages( const MicroVulkanDevice& device );

	void DestroyImages( const MicroVulkanDevice& device );

public:
	const MicroVulkanSwapchainSpecification& GetSpecification( ) const;

	VkSwapchainKHR Get( ) const;

	const std::vector<MicroVulkanSwapchainImage> GetImages( ) const;

private:
	VkSurfaceCapabilitiesKHR GetSurfaceSpec(
		const MicroVulkanInstance& instance,
		const MicroVulkanDevice& device
	) const;

	VkSurfaceFormatKHR GetSurfaceFormat( 
		const MicroVulkanInstance& instance,
		const MicroVulkanDevice& device 
	) const;

	void GetQueueSharingPolicy(
		const MicroVulkanDevice& device,
		VkSwapchainCreateInfoKHR& specification
	);

	void GetSwapchainSpecification( 
		const VkSwapchainCreateInfoKHR& swapchain_info 
	);

public:
	operator VkSwapchainKHR ( ) const;

	operator VkSwapchainKHR* ( ) const;

};
