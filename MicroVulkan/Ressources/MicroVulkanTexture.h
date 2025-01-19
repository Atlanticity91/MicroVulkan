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

#include "../Device/MicroVulkanCommands.h"

#define MVK_IMAGE_USAGE_TEXTURE ( VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT )
#define MVK_IMAGE_USAGE_COLOR   ( VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT )
#define MVK_IMAGE_USAGE_DEPTH   ( VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT )

struct MicroTextureProperties {

	VkFormat Format = VK_FORMAT_UNDEFINED;
	VkImageLayout Layout = VK_IMAGE_LAYOUT_UNDEFINED;
	VkExtent3D Extent{ };
	uint32_t MipLevels = 1;
	uint32_t ArrayLayers = 1;

};

struct MicroVulkanTextureSpecification {

	MicroTextureProperties Properties{
		VK_FORMAT_R8G8B8A8_SRGB,
		VK_IMAGE_LAYOUT_GENERAL,
		{ 0, 0, 0 },
		1,
		1
	};

	VkBool32 IsCubemap			  = VK_FALSE;
	VkImageType Type			  = VK_IMAGE_TYPE_2D;
	VkSampleCountFlagBits Samples = VK_SAMPLE_COUNT_1_BIT;
	VkImageUsageFlags Usage		  = VK_IMAGE_USAGE_SAMPLED_BIT;
	VkBool32 UseSampler			  = VK_TRUE;
	VkSamplerCreateInfo Sampler{ };

};

class MicroVulkanTexture {

private:
	VkImage m_image;
	VkDeviceMemory m_memory;
	VkImageView m_view;
	VkSampler m_sampler;

public:
	MicroVulkanTexture( );

	~MicroVulkanTexture( ) = default;

	bool Create( const MicroVulkanSwapchainImage& image );

	bool Create(
		const MicroVulkanDevice& device,
		const MicroVulkanQueues& queues,
		const MicroVulkanTextureSpecification& specification
	);

	void Destroy( const MicroVulkanDevice& device );

private:
	bool CreateImage(
		const MicroVulkanDevice& device,
		const MicroVulkanQueues& queues,
		const MicroVulkanTextureSpecification& specification
	);

	bool CreateStorage( 
		const MicroVulkanDevice& device
	);

	bool CreateImageView( 
		const MicroVulkanDevice& device,
		const MicroVulkanTextureSpecification& specification
	);

	bool CreateImageSampler( 
		const MicroVulkanDevice& device,
		const MicroVulkanTextureSpecification& specification
	);

public:
	VkImage GetImage( ) const;

	VkImageView GetView( ) const;
	
	VkSampler GetSampler( ) const;

private:
	void GetQueueSharingPolicy(
		const MicroVulkanQueues& queues,
		VkImageCreateInfo& image_spec
	) const;

	VkImageViewType GetImageViewType( 
		const MicroVulkanTextureSpecification& specification
	) const;

	VkImageAspectFlagBits GetImageAspect(
		const MicroVulkanTextureSpecification& specification
	) const;

public:
	operator VkImage ( ) const;

};
