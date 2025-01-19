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

/**
 * MicroVulkanBackend final class
 * @note : Defined wrapper for micro vulkan device and render control objects.
 **/
class MicroVulkanBackend final {

private:
	uint32_t m_frame_id;
	uint32_t m_frame_count;
	MicroVulkanInstance m_instance;
	MicroVulkanDevice m_device;
	mutable MicroVulkanQueues m_queues;
	MicroVulkanSwapchain m_swapchain;
	MicroVulkanRenderPasses m_passes;
	MicroVulkanSynchronization m_synchronization;
	mutable MicroVulkanStagings m_stagings;
	mutable MicroVulkanCommands m_commands;
	MicroVulkanFramebuffers m_framebuffers;
	MicroVulkanPipelines m_pipeline_cache;

public:
	/**
	 * Constructor
	 **/
	MicroVulkanBackend( );

	/**
	 * Destructor
	 **/
	~MicroVulkanBackend( ) = default;
	
	/**
	 * Create function
	 * @note : Setup the vulkan backend.
	 * @param window : Reference to current micro window instance.
	 * @param specification : Reference to custom backend specification.
	 * @return : True when vulkan backend setup succeeded.
	 **/
	bool Create( 
		const MicroWindow& window, 
		const MicroVulkanSpecification& specification
	);

	/**
	 * Wait method
	 * @note : Wait for vulkan device to be idle, same effect as vkDeviceWaitIdle.
	 **/
	void Wait( );

	/**
	 * Acquire function
	 * note : Acquire render context and setup backend for rendering.
	 * @param window : Reference to current micro window instance.
	 * @param render_context : Reference to render context to fill.
	 * @param need_resize : Reference to need resize variable, used to control 
	 * 						externaly if backend need to be resized.
	 * @return : True when Acquire succeeded and render context can be used for rendering.
	 **/
	bool Acquire( 
		const MicroWindow& window, 
		MicroVulkanRenderContext& render_context,
		bool& need_resize
	);

	/**
	 * CreateRenderPassInfo function
	 * @note : Create render pass information.
	 * @param render_context : Reference to current render context instance.
	 * @param pass_id : Query render pass index.
	 * @return : Return the vulkan begin pass info required to use a pass during rendering.
	 **/
	VkRenderPassBeginInfo CreateRenderPassInfo(
		const MicroVulkanRenderContext& render_context,
		const uint32_t pass_id
	);

	/**
	 * Present method
	 * @note : Present rendering setup by backend to screen.
	 * @param window : Reference to current micro window instance.
	 * @param render_context : Reference to render context to fill.
	 * @param need_resize : Reference to need resize variable, used to control 
	 * 						externaly if backend need to be resized.
	 **/
	void Present( 
		const MicroWindow& window,
		MicroVulkanRenderContext& render_context,
		bool& need_resize
	);

	/**
	 * Destroy method
	 * @note : Destroy and cleanup vulkan backend.
	 **/
	void Destroy( );

private:
	/**
	 * CreateInstance function
	 * @note : Create micro vulkan instance wrapper.
	 * @param window : Reference to current micro window instance.
	 * @param specification : Reference to custom backend specification.
	 * @return : True when instance creation succeeded.
	 **/
	bool CreateInstance(
		const MicroWindow& window,
		const MicroVulkanSpecification& specification
	);

	/**
	 * CreatePipelines function
	 * @note : Create pipeline manager & vulkan pipeline cache.
	 * @param window : Reference to current micro window instance.
	 * @param specification : Reference to custom backend specification.
	 * @return : True when pipeline manager creation succeeded.
	 **/
	bool CreatePipelines(
		const MicroWindow& window,
		const MicroVulkanSpecification& specification
	);

	/**
	 * CreateRenderContext function
	 * @note : Create render conctext specification for rendering.
	 * @param render_context : Reference to render context to fill.
	 * @return : Vulkan result value.
	 **/
	VkResult CreateRenderContext( MicroVulkanRenderContext& render_context );

	/**
	 * CreatePresentSpec method.
	 * @note : Create vulkan presentation information structure instance.
	 * @param render_context : Reference to current render context.
	 * @return : Vulkan present info instance.
	 **/
	VkPresentInfoKHR CreatePresentSpec( const MicroVulkanRenderContext& render_context );

	/**
	 * DestroyRenderContext method
	 * @note : Destroy render conctext specification.
	 * @param render_context : Reference to render context to destroy.
	 **/
	void DestroyRenderContext( MicroVulkanRenderContext& render_context );

	/**
	 * Recreate method
	 * @note : Recreate vulkan backend to fit new window sepcification.
	 * @param window : Reference to current micro window instance.
	 **/
	void Recreate( const MicroWindow& window );

public:
	/**
	 * GetInstance const function
	 * @note : Get current micro vulkan instance wrapper.
	 * @return : Reference to instance wrapper.
	 **/
	const MicroVulkanInstance& GetInstance( ) const;

	/**
	 * GetDevice const function
	 * @note : Get current micro vulkan device wrapper.
	 * @return : Reference to device wrapper.
	 **/
	const MicroVulkanDevice& GetDevice( ) const;

	/**
	 * GetQueues const function
	 * @note : Get current micro vulkan device queues wrapper.
	 * @return : Reference to device queues wrapper.
	 **/
	MicroVulkanQueues& GetQueues( ) const;

	/**
	 * GetSwapchain const function
	 * @note : Get current micro vulkan swapchain wrapper.
	 * @return : Reference to swapchain wrapper.
	 **/
	const MicroVulkanSwapchain& GetSwapchain( ) const;

	/**
	 * GetPasses const function
	 * @note : Get current micro vulkan render pass manager.
	 * @return : Reference to render pass manager.
	 **/
	const MicroVulkanRenderPasses& GetPasses( ) const;

	/**
	 * GetStaging const function
	 * @note : Get current micro vulkan staging buffer manager wrapper.
	 * @return : Reference to staging buffer manager.
	 **/
	MicroVulkanStagings& GetStaging( ) const;

	/**
	 * GetCommands const function
	 * @note : Get current micro vulkan command buffer manager wrapper.
	 * @return : Reference to command buffer manager.
	 **/
	MicroVulkanCommands& GetCommands( ) const;

	/**
	 * GetFramebuffer const function
	 * @note : Get render frame buffer wrapper for a specific render pass.
	 * @param render_pass_id : Query render pass index.
	 * @return : Reference to render frame buffer wrapper.
	 **/
	const MicroVulkanFramebuffer& GetFramebuffer( const uint32_t render_pass_id ) const;

	/**
	 * GetFramebufferDimensions const function
	 * @note : Get render pass frame buffer dimensions.
	 * @param render_pass_id : Query render pass index.
	 * @return : Frame buffer dimensions as unsigned integer point.
	 **/
	micro_upoint GetFramebufferDimensions( const uint32_t render_pass_id ) const;

	/**
	 * GetFramebufferClearValues function
	 * @note : Get render pass frame buffer attachement clear values.
	 * @param render_pass_id : Query render pass index.
	 * @return : Vector of frame buffer attachement clear values.
	 **/
	std::vector<VkClearValue>& GetFramebufferClearValues( const uint32_t render_pass_id );

	/**
	 * GetFramebufferTargets const function
	 * @note : Get all frame buffer target wrapper for a render pass.
	 * @param render_pass_id : Query render pass index.
	 * @return : Vector of all frame buffer target.
	 **/
	const std::vector<MicroVulkanFrameTarget>& GetFramebufferTargets(
		const uint32_t render_pass_id
	) const;

	/**
	 * GetFramebufferTarget const function
	 * @note : Get a frame buffer target wrapper for a render pass.
	 * @param render_pass_id : Query render pass index.
	 * @param frame_id : Query frame target index.
	 * @return : Reference to frame buffer target.
	 **/
	const MicroVulkanFrameTarget& GetFramebufferTarget(
		const uint32_t render_pass_id,
		const uint32_t frame_id
	) const;

	/**
	 * GetPipelines const function
	 * @note : Get current micro vulkan pipeline manager.
	 * @return : Reference to pipeline manager.
	 **/
	const MicroVulkanPipelines& GetPipelines( ) const;

	/**
	 * GetPipelineCache const function
	 * @note : Get current vulkan pipeline cache.
	 * @return : Reference to vulkan pipeline cache.
	 **/
	const VkPipelineCache& GetPipelineCache( ) const;

};
