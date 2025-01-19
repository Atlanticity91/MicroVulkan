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

#include "../Ressources/MicroVulkanTexture.h"

micro_struct MicroVulkanFrameTargetTexture {

	MicroVulkanTexture Texture{ };
	VkFormat Format = VK_FORMAT_UNDEFINED;

	bool Create( const MicroVulkanSwapchainImage& swapchain_image ) {
		return Texture.Create( swapchain_image );
	};

	bool Create(
		const MicroVulkanDevice& device,
		const MicroVulkanQueues& queues,
		const MicroVulkanTextureSpecification& specification
	) {
		Format = specification.Properties.Format;

		return Texture.Create( device, queues, specification );
	};

	void Destroy( const MicroVulkanDevice& device ) {
		Texture.Destroy( device );
	};

	VkImageView GetView( ) const {
		return Texture.GetView( );
	};

};

micro_struct MicroVulkanFrameTarget {

	VkFramebuffer Framebuffer = VK_NULL_HANDLE;
	std::vector<MicroVulkanFrameTargetTexture> Textures{ };

	operator VkFramebuffer ( ) const {
		return Framebuffer;
	};

};

micro_struct MicroVulkanFramebuffer {

	micro_upoint Dimensions{ 0, 0 };
	std::vector<VkClearValue> Clears{ };
	std::vector<MicroVulkanFrameTarget> Targets{ };

};

micro_struct MicroVulkanRenderPassInfo {

	VkRenderPassBeginInfo BeginInfo;
	VkViewport Viewport;
	VkRect2D Scissor;

	MicroVulkanRenderPassInfo( ) 
		: BeginInfo{ },
		Viewport{ },
		Scissor{ }
	{ };

	MicroVulkanRenderPassInfo( MicroVulkanRenderPassInfo&& other ) 
	{ 
		memcpy_s( this, sizeof( MicroVulkanRenderPassInfo ), &other, sizeof( MicroVulkanRenderPassInfo ) );
	};

};

class MicroVulkanFramebuffers final {

private:
	MicroVulkanDimensionsPolicy m_dimensions_policy;
	std::vector<MicroVulkanFramebuffer> m_framebuffers;

public:
	MicroVulkanFramebuffers( );

	~MicroVulkanFramebuffers( ) = default;

	bool Create(
		const MicroWindow& window,
		const MicroVulkanDevice& device,
		const MicroVulkanQueues& queues,
		const MicroVulkanSwapchain& swapchain,
		const MicroVulkanRenderPasses& passes,
		const MicroVulkanSpecification& specification
	);

	void Recreate( 
		const MicroVulkanDevice& device,
		const MicroVulkanQueues& queues,
		const MicroVulkanSwapchain& swapchain,
		const MicroVulkanRenderPasses& passes,
		const micro_upoint& dimensions
	);

	void SetClearValue(
		const uint32_t render_pass_id,
		const uint32_t image_id,
		const VkClearValue& value
	);

	void SetClearValue( 
		const uint32_t render_pass_id,
		const std::vector<VkClearValue>& values
	);

	MicroVulkanRenderPassInfo CreateRenderPassInfo(
		const uint32_t render_pass_id,
		const uint32_t frame_id,
		const VkRenderPass render_pass
	);

	void Destroy( const MicroVulkanDevice& device );

private:
	micro_upoint CreateDimensionsSpec( const MicroWindow& window );

	micro_upoint CreateDimensionsSpec( const micro_upoint& dimensions );

	void CreateTextureSampler( 
		MicroVulkanTextureSpecification& texture_spec
	);

	MicroVulkanTextureSpecification CreateTextureSpec( 
		const micro_upoint& dimensions,
		const VkFormat format
	);

	bool CreateFramebufferSwapchain(
		const MicroVulkanDevice& device, 
		const MicroVulkanQueues& queues,
		const std::vector<VkAttachmentDescription>& attachements_spec,
		const MicroVulkanSwapchainImage& swapchain_image,
		const micro_upoint& dimensions,
		MicroVulkanFrameTarget& target
	);

	bool CreateFramebufferTextures(
		const MicroVulkanDevice& device,
		const MicroVulkanQueues& queues,
		const std::vector<VkAttachmentDescription>& attachements_spec,
		const micro_upoint& dimensions,
		MicroVulkanFrameTarget& target
	);

	void RecreateFramebufferTextures(
		const MicroVulkanDevice& device,
		const MicroVulkanQueues& queues,
		const MicroVulkanSwapchainImage& image,
		const uint32_t render_pass_id,
		const micro_upoint& dimensions,
		MicroVulkanFrameTarget& target
	);

	std::vector<VkImageView> CreateFramebufferAttachements(
		const MicroVulkanFrameTarget& target
	);

	VkFramebufferCreateInfo CreateFramebufferSpec(
		const micro_upoint& dimensions,
		const VkRenderPass& render_pass,
		const std::vector<VkImageView>& attachements
	);

	bool CreateFramebuffer( 
		const MicroVulkanDevice& device,
		const VkRenderPass& render_pass,
		const micro_upoint& dimensions,
		MicroVulkanFrameTarget& target
	);

	bool CreateFramebuffers( 
		const MicroVulkanDevice& device,
		const MicroVulkanQueues& queues,
		const MicroVulkanSwapchain& swapchain,
		const MicroVulkanRenderPasses& passes,
		const MicroVulkanSpecification& specification,
		const micro_upoint& dimensions
	);

	void RecreateFramebuffers(
		const MicroVulkanDevice& device,
		const MicroVulkanQueues& queues,
		const MicroVulkanSwapchain& swapchain,
		const MicroVulkanRenderPasses& passes,
		const micro_upoint& dimensions
	);

	void DestroyFramebuffer( 
		const MicroVulkanDevice& device,
		const uint32_t frame_id,
		MicroVulkanFrameTarget& target
	);

public:
	const MicroVulkanFramebuffer& Get( const uint32_t render_pass_id ) const;

	micro_upoint GetDimensions( const uint32_t render_pass_id ) const;

	std::vector<VkClearValue>& GetClearValues( const uint32_t render_pass_id );

	const std::vector<MicroVulkanFrameTarget>& GetTargets(
		const uint32_t render_pass_id
	) const;

	const MicroVulkanFrameTarget& GetTarget(
		const uint32_t render_pass_id,
		const uint32_t frame_id
	) const;

private:
	VkImageLayout GetTextureLayout( const VkFormat format ) const;

	VkImageUsageFlags GetTextureUsage( const VkFormat format ) const;

	void GetRenderFramebuffersSpec( const MicroVulkanSpecification& specification );

};
