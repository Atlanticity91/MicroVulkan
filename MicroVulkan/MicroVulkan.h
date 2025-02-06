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

#include "Rendering/MicroVulkanRenderContext.h"

micro_class MicroVulkan final { 

private:
	uint32_t m_frame_id;
	uint32_t m_frame_count;
	MicroVulkanInstance m_instance;
	MicroVulkanDevice m_device;
	MicroVulkanQueues m_queues;
	MicroVulkanSwapchain m_swapchain;
	MicroVulkanRenderPasses m_passes;
	MicroVulkanSynchronization m_synchronization;
	MicroVulkanStagings m_stagings;
	MicroVulkanCommands m_commands;
	MicroVulkanFramebuffers m_framebuffers;
	MicroVulkanPipelines m_pipeline_cache;

public:
	MicroVulkan( );

	~MicroVulkan( ) = default;

	bool Create( 
		const MicroVulkanWindow& window,
		const MicroVulkanSpecification& specification
	);

	void Wait( );

	bool Acquire( 
		const MicroVulkanWindow& window,
		MicroVulkanRenderContext& render_context,
		bool& need_resize
	);

	MicroVulkanRenderPassInfo CreateRenderPassInfo(
		const MicroVulkanRenderContext& render_context,
		const uint32_t pass_id
	);

	void Present( 
		const MicroVulkanWindow& window,
		MicroVulkanRenderContext& render_context,
		bool& need_resize
	);

	void Destroy( );

private:
	void CreateValidationSpec( MicroVulkanSpecification& specification );

	void CreateInstanceExtensionsSpec( 
		const MicroVulkanWindow& window,
		MicroVulkanSpecification& specification 
	);

	void CreateDeviceExtensionsSpec( MicroVulkanSpecification& specification );

	void CreateExtensionsSpec(
		const MicroVulkanWindow& window,
		MicroVulkanSpecification& specification 
	);

	bool CreateInstance(
		const MicroVulkanWindow& window,
		const MicroVulkanSpecification& specification
	);

	bool CreatePipelines(
		const MicroVulkanWindow& window,
		const MicroVulkanSpecification& specification
	);

	VkResult CreateRenderContext( MicroVulkanRenderContext& render_context );

	VkPresentInfoKHR CreatePresentSpec(
		const MicroVulkanRenderContext& render_context
	);

	void DestroyRenderContext( MicroVulkanRenderContext& render_context );

	void Recreate( const MicroVulkanWindow& window );

public:
	const MicroVulkanInstance& GetInstance( ) const;

	const MicroVulkanDevice& GetDevice( ) const;

	MicroVulkanQueues& GetQueues( );

	const MicroVulkanQueues& GetQueues( ) const;

	const MicroVulkanSwapchain& GetSwapchain( ) const;

	const MicroVulkanRenderPasses& GetPasses( ) const;

	MicroVulkanStagings& GetStaging( );

	const MicroVulkanStagings& GetStaging( ) const;

	MicroVulkanCommands& GetCommands( );

	const MicroVulkanCommands& GetCommands( ) const;

	const MicroVulkanFramebuffer& GetFramebuffer(
		const uint32_t render_pass_id
	) const;

	micro_upoint GetFramebufferDimensions( const uint32_t render_pass_id ) const;

	const std::vector<VkClearValue>& GetFramebufferClearValues(
		const uint32_t render_pass_id 
	);

	const std::vector<MicroVulkanFrameTarget>& GetFramebufferTargets(
		const uint32_t render_pass_id
	) const;

	const MicroVulkanFrameTarget& GetFramebufferTarget(
		const uint32_t render_pass_id,
		const uint32_t frame_id
	) const;

	const MicroVulkanPipelines& GetPipelines( ) const;

	const VkPipelineCache& GetPipelineCache( ) const;

};
