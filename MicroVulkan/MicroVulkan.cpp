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
MicroVulkan::MicroVulkan( )
	: m_frame_id{ 0 },
	m_frame_count{ 0 },
	m_instance{ },
	m_device{ },
	m_queues{ },
	m_swapchain{ },
	m_passes{ },
	m_synchronization{ },
	m_stagings{ },
	m_commands{ },
	m_framebuffers{ },
	m_pipeline_cache{ }
{ }

bool MicroVulkan::Create(
	const MicroVulkanWindow& window,
	const MicroVulkanSpecification& specification
) {
	auto state = false;
	auto spec  = MicroVulkanSpecification{ specification };

	CreateExtensionsSpec( window, spec );

	if ( CreateInstance( window, spec ) ) {
		m_frame_count = m_swapchain.GetSpecification( ).ImageCount;

		m_queues.Create( m_device );

		state = CreatePipelines( window, spec );
	}

	return state;
}

void MicroVulkan::Wait( ) {
	m_device.Wait( );
}

bool MicroVulkan::Acquire( 
	const MicroVulkanWindow& window,
	MicroVulkanRenderContext& render_context,
	bool& need_resize
) {
	auto success = true;
	auto result  = CreateRenderContext( render_context );
	
	if ( need_resize || result == VK_ERROR_OUT_OF_DATE_KHR ) {
		Recreate( window );

		need_resize = false;
		success		= false;
	} else if ( result == VK_SUCCESS || result == VK_SUBOPTIMAL_KHR )
		vkResetFences( m_device, 1, micro_ptr( render_context.Sync->Signal ) );
	else 
		throw std::runtime_error( "[ VK - FAIL ] Swapchain image acquisition failed" );

	return success;
}

MicroVulkanRenderPassInfo MicroVulkan::CreateRenderPassInfo(
	const MicroVulkanRenderContext& render_context,
	const uint32_t pass_id
) {
	auto render_pass = m_passes.Get( pass_id );
	auto pass_info   = m_framebuffers.CreateRenderPassInfo( pass_id, render_context.FrameID, render_pass );

	return pass_info;
}

void MicroVulkan::Present( 
	const MicroVulkanWindow& window,
	MicroVulkanRenderContext& render_context,
	bool& need_resize
) {
	auto specification = CreatePresentSpec( render_context );
	auto result		   = vkQueuePresentKHR( render_context.Queue, micro_ptr( specification ) );

	if ( need_resize || result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ) {
		Recreate( window );

		need_resize = false;
	} else if ( result != VK_SUCCESS )
		throw std::runtime_error( "[ VK - FAIL ] Swapchain image presentation failed" );

	DestroyRenderContext( render_context );

	m_frame_id = ( m_frame_id + 1 ) % m_frame_count;
}

void MicroVulkan::Destroy( ) {
	m_device.Wait( );

	m_pipeline_cache.Destroy( m_device );
	m_framebuffers.Destroy( m_device );
	m_commands.Destroy( m_device );
	m_stagings.Destroy( m_device );
	m_synchronization.Destroy( m_device );
	m_passes.Destroy( m_device );
	m_swapchain.Destroy( m_device );
	m_queues.Destroy( );
	m_device.Destroy( );
	m_instance.Destroy( );
}

////////////////////////////////////////////////////////////////////////////////////////////
//		===	PRIVATE ===
////////////////////////////////////////////////////////////////////////////////////////////
void MicroVulkan::CreateValidationSpec( MicroVulkanSpecification& specification ) {
	auto& spec = specification.Validations;

#	ifdef _DEBUG 
	spec.emplace_back( "VK_LAYER_KHRONOS_validation" );
#	endif
}

void MicroVulkan::CreateInstanceExtensionsSpec( 
	const MicroVulkanWindow& window,
	MicroVulkanSpecification& specification 
) {
	window.GetExtensions( specification.InstanceExtensions );

	specification.InstanceExtensions.emplace_back( VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME );

#	ifdef _DEBUG 
	specification.InstanceExtensions.emplace_back( VK_EXT_DEBUG_UTILS_EXTENSION_NAME );
#	endif
}

void MicroVulkan::CreateDeviceExtensionsSpec( MicroVulkanSpecification& specification ) {
	specification.DeviceExtensions.emplace_back( VK_KHR_MAINTENANCE3_EXTENSION_NAME );
	specification.DeviceExtensions.emplace_back( VK_KHR_SWAPCHAIN_EXTENSION_NAME );
}

void MicroVulkan::CreateExtensionsSpec(
	const MicroVulkanWindow& window,
	MicroVulkanSpecification& specification
) {
	CreateValidationSpec( specification );
	CreateInstanceExtensionsSpec( window, specification );
	CreateDeviceExtensionsSpec( specification );
}

bool MicroVulkan::CreateInstance(
	const MicroVulkanWindow& window,
	const MicroVulkanSpecification& specification
) {
	return  m_instance.Create( window, specification )		   &&
			m_device.Create( m_instance, specification )	   &&
			m_swapchain.Create( window, m_instance, m_device ) &&
			m_passes.Create( m_device, specification )		   &&
			m_synchronization.Create( m_device, m_swapchain )  &&
			m_stagings.Create( m_device );
}

bool MicroVulkan::CreatePipelines(
	const MicroVulkanWindow& window,
	const MicroVulkanSpecification& specification
) {
	return  m_commands.Create( m_device, m_queues, m_swapchain )									  &&
			m_framebuffers.Create( window, m_device, m_queues, m_swapchain, m_passes, specification ) &&
			m_pipeline_cache.Create( m_device, specification );
}

VkResult MicroVulkan::CreateRenderContext(
	MicroVulkanRenderContext& render_context
) {
	render_context.FrameID		 = m_frame_id;
	render_context.Sync			 = m_synchronization.Acquire( m_frame_id );
	render_context.Queue		 = m_queues.Acquire( vk::QUEUE_TYPE_GRAPHICS );
	render_context.CommandBuffer = m_commands.Acquire( vk::QUEUE_TYPE_GRAPHICS, VK_COMMAND_BUFFER_LEVEL_PRIMARY );

	vk::WaitForFences( m_device, render_context.Sync->Signal, UINT64_MAX );
	
	return vkAcquireNextImageKHR( m_device, m_swapchain, UINT64_MAX, render_context.Sync->Presentable, VK_NULL_HANDLE, micro_ptr( render_context.ImageID ) );
}

VkPresentInfoKHR MicroVulkan::CreatePresentSpec(
	const MicroVulkanRenderContext& render_context
) {
	auto specification = VkPresentInfoKHR{ };

	specification.sType				 = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	specification.pNext				 = VK_NULL_HANDLE;
	specification.waitSemaphoreCount = 1;
	specification.pWaitSemaphores	 = micro_ptr( render_context.Sync->Renderable );
	specification.swapchainCount	 = 1;
	specification.pSwapchains		 = m_swapchain;
	specification.pImageIndices		 = micro_ptr( render_context.ImageID );
	specification.pResults			 = VK_NULL_HANDLE;

	return specification;
}

void MicroVulkan::DestroyRenderContext( MicroVulkanRenderContext& render_context ) {
	m_commands.Release( render_context.CommandBuffer );
	m_queues.Release( render_context.Queue );
}

void MicroVulkan::Recreate( const MicroVulkanWindow& window ) {
	auto dimensions = window.GetDimensions( );

	m_device.Wait( );

	m_swapchain.Recreate( m_instance, m_device, dimensions );
	m_framebuffers.Recreate( m_device, m_queues, m_swapchain, m_passes, dimensions );

	m_device.Wait( );
}

////////////////////////////////////////////////////////////////////////////////////////////
//		===	PUBLIC GET ===
////////////////////////////////////////////////////////////////////////////////////////////
const MicroVulkanInstance& MicroVulkan::GetInstance( ) const {
	return m_instance;
}

const MicroVulkanDevice& MicroVulkan::GetDevice( ) const {
	return m_device;
}

MicroVulkanQueues& MicroVulkan::GetQueues( ) {
	return m_queues;
}

const MicroVulkanQueues& MicroVulkan::GetQueues( ) const {
	return m_queues;
}

const MicroVulkanSwapchain& MicroVulkan::GetSwapchain( ) const {
	return m_swapchain;
}

const MicroVulkanRenderPasses& MicroVulkan::GetPasses( ) const {
	return m_passes;
}

const MicroVulkanStagings& MicroVulkan::GetStaging( ) const {
	return m_stagings;
}

MicroVulkanStagings& MicroVulkan::GetStaging( ) {
	return m_stagings;
}

MicroVulkanCommands& MicroVulkan::GetCommands( ) {
	return m_commands;
}

const MicroVulkanCommands& MicroVulkan::GetCommands( ) const {
	return m_commands;
}

const MicroVulkanFramebuffer& MicroVulkan::GetFramebuffer(
	const uint32_t render_pass_id 
) const {
	return m_framebuffers.Get( render_pass_id );
}

micro_upoint MicroVulkan::GetFramebufferDimensions(
	const uint32_t render_pass_id 
) const {
	return m_framebuffers.GetDimensions( render_pass_id );
}

const std::vector<VkClearValue>& MicroVulkan::GetFramebufferClearValues(
	const uint32_t render_pass_id 
) {
	return m_framebuffers.GetClearValues( render_pass_id );
}

const std::vector<MicroVulkanFrameTarget>& MicroVulkan::GetFramebufferTargets(
	const uint32_t render_pass_id
) const {
	return m_framebuffers.GetTargets( render_pass_id );
}

const MicroVulkanFrameTarget& MicroVulkan::GetFramebufferTarget(
	const uint32_t render_pass_id,
	const uint32_t frame_id
) const {
	return m_framebuffers.GetTarget( render_pass_id, frame_id );
}

const MicroVulkanPipelines& MicroVulkan::GetPipelines( ) const {
	return m_pipeline_cache;
}

const VkPipelineCache& MicroVulkan::GetPipelineCache( ) const {
	return m_pipeline_cache.GetCache( );
}
