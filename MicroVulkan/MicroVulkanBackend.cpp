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

#include <__micro_vulkan_pch.h>

////////////////////////////////////////////////////////////////////////////////////////////
//		===	PUBLIC ===
////////////////////////////////////////////////////////////////////////////////////////////
MicroVulkanBackend::MicroVulkanBackend( )
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

bool MicroVulkanBackend::Create(
	const MicroWindow& window,
	const MicroVulkanSpecification& specification
) {
	auto state = false;

	if ( CreateInstance( window, specification ) ) {
		m_frame_count = m_swapchain.GetSpecification( ).ImageCount;

		m_queues.Create( m_device );

		state = CreatePipelines( window, specification );
	}

	return state;
}

void MicroVulkanBackend::Wait( ) {
	m_device.Wait( );
}

bool MicroVulkanBackend::Acquire(
	const MicroWindow& window,
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
		vkResetFences( m_device, 1, &render_context.Sync->Signal );
	else 
		throw std::runtime_error( "[ VK - FAIL ] Swapchain image acquisition failed" );

	return success;
}

VkRenderPassBeginInfo MicroVulkanBackend::CreateRenderPassInfo(
	const MicroVulkanRenderContext& render_context,
	const uint32_t pass_id
) {
	auto render_pass_info = m_framebuffers.CreateRenderPassInfo( pass_id, render_context.FrameID );

	render_pass_info.renderPass = m_passes.Get( pass_id );

	return render_pass_info;
}

void MicroVulkanBackend::Present(
	const MicroWindow& window,
	MicroVulkanRenderContext& render_context,
	bool& need_resize
) {
	auto specification = CreatePresentSpec( render_context );
	auto result		   = vkQueuePresentKHR( render_context.Queue, &specification );

	if ( need_resize || result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ) {
		Recreate( window );

		need_resize = false;
	} else if ( result != VK_SUCCESS )
		throw std::runtime_error( "[ VK - FAIL ] Swapchain image presentation failed" );

	DestroyRenderContext( render_context );

	m_frame_id = ( m_frame_id + 1 ) % m_frame_count;
}

void MicroVulkanBackend::Destroy( ) {
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
/*
void MicroVulkan::CreateApplicationSpec( MicroVulkanSpecification& specification ) {
	auto& spec = specification.Application;

	spec.sType				= VK_STRUCTURE_TYPE_APPLICATION_INFO;
	spec.pNext				= VK_NULL_HANDLE;
	spec.pApplicationName   = "Micro Game";
	spec.applicationVersion = VK_MAKE_VERSION( 1, 0, 0 );
	spec.pEngineName		= "Micro Vulkan";
	spec.engineVersion		= VK_MAKE_VERSION( 1, 0, 0 );
	spec.apiVersion			= VK_API_VERSION_1_3;
}

void MicroVulkan::CreateValidationSpec( MicroVulkanSpecification& specification ) {
	auto& spec = specification.Validations;

#	ifdef _DEBUG 
	spec.emplace_back( "VK_LAYER_KHRONOS_validation" );
#	endif
}

void MicroVulkan::CreateInstanceExtensionsSpec( 
	const MicroWindow& window, 
	MicroVulkanSpecification& specification 
) {
	auto& spec = specification.InstanceExtensions;
	auto count = (uint32_t)0;

	SDL_Vulkan_GetInstanceExtensions( window, &count, VK_NULL_HANDLE );

	specification.InstanceExtensions.resize( count );

	SDL_Vulkan_GetInstanceExtensions( window, &count, spec.data( ) );

	spec.emplace_back( VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME );

#	ifdef _DEBUG 
	spec.emplace_back( VK_EXT_DEBUG_UTILS_EXTENSION_NAME );
#	endif
}

void MicroVulkan::CreateDeviceExtensionsSpec( MicroVulkanSpecification& specification ) {
	auto& spec = specification.DeviceExtensions;

	spec.emplace_back( VK_KHR_MAINTENANCE3_EXTENSION_NAME );
	spec.emplace_back( VK_KHR_SWAPCHAIN_EXTENSION_NAME );
}
specification.Depths.emplace_back( VK_FORMAT_D24_UNORM_S8_UINT );
*/

bool MicroVulkanBackend::CreateInstance(
	const MicroWindow& window,
	const MicroVulkanSpecification& specification
) {
	return  m_instance.Create( window, specification )		   &&
			m_device.Create( m_instance, specification )	   &&
			m_swapchain.Create( window, m_instance, m_device ) &&
			m_passes.Create( m_device, specification )		   &&
			m_synchronization.Create( m_device, m_swapchain )  &&
			m_stagings.Create( m_device );
}

bool MicroVulkanBackend::CreatePipelines(
	const MicroWindow& window,
	const MicroVulkanSpecification& specification
) {
	return  m_commands.Create( m_device, m_queues, m_swapchain )							&&
			m_framebuffers.Create( window, m_device, m_queues, m_swapchain, m_passes, specification ) &&
			m_pipeline_cache.Create( m_device, specification );
}

VkResult MicroVulkanBackend::CreateRenderContext(
	MicroVulkanRenderContext& render_context
) {
	render_context.FrameID		 = m_frame_id;
	render_context.Sync			 = m_synchronization.Acquire( m_frame_id );
	render_context.Queue		 = m_queues.Acquire( vk::QUEUE_TYPE_GRAPHICS );
	render_context.CommandBuffer = m_commands.Acquire( vk::QUEUE_TYPE_GRAPHICS, VK_COMMAND_BUFFER_LEVEL_PRIMARY );

	vk::WaitForFences( m_device, render_context.Sync->Signal, UINT64_MAX );
	
	return vkAcquireNextImageKHR( m_device, m_swapchain, UINT64_MAX, render_context.Sync->Presentable, VK_NULL_HANDLE, &render_context.ImageID );
}

VkPresentInfoKHR MicroVulkanBackend::CreatePresentSpec(
	const MicroVulkanRenderContext& render_context
) {
	auto specification = VkPresentInfoKHR{ };

	specification.sType				 = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	specification.pNext				 = VK_NULL_HANDLE;
	specification.waitSemaphoreCount = 1;
	specification.pWaitSemaphores	 = &render_context.Sync->Renderable;
	specification.swapchainCount	 = 1;
	specification.pSwapchains		 = m_swapchain;
	specification.pImageIndices		 = &render_context.ImageID;
	specification.pResults			 = VK_NULL_HANDLE;

	return specification;
}

void MicroVulkanBackend::DestroyRenderContext( MicroVulkanRenderContext& render_context ) {
	m_commands.Release( render_context.CommandBuffer );
	m_queues.Release( render_context.Queue );
}

void MicroVulkanBackend::Recreate( const MicroWindow& window ) {
	auto dimensions = window.GetDimensions( );

	m_device.Wait( );

	m_swapchain.Recreate( m_instance, m_device, dimensions );
	m_framebuffers.Recreate( m_device, m_queues, m_swapchain, m_passes, dimensions );

	m_device.Wait( );
}


////////////////////////////////////////////////////////////////////////////////////////////
//		===	PUBLIC GET ===
////////////////////////////////////////////////////////////////////////////////////////////
const MicroVulkanInstance& MicroVulkanBackend::GetInstance( ) const {
	return m_instance;
}

const MicroVulkanDevice& MicroVulkanBackend::GetDevice( ) const {
	return m_device;
}

MicroVulkanQueues& MicroVulkanBackend::GetQueues( ) const {
	return m_queues;
}

const MicroVulkanSwapchain& MicroVulkanBackend::GetSwapchain( ) const {
	return m_swapchain;
}

const MicroVulkanRenderPasses& MicroVulkanBackend::GetPasses( ) const {
	return m_passes;
}

MicroVulkanStagings& MicroVulkanBackend::GetStaging( ) const {
	return m_stagings;
}

MicroVulkanCommands& MicroVulkanBackend::GetCommands( ) const {
	return m_commands;
}

const MicroVulkanFramebuffer& MicroVulkanBackend::GetFramebuffer(
	const uint32_t render_pass_id 
) const {
	return m_framebuffers.Get( render_pass_id );
}

micro_upoint MicroVulkanBackend::GetFramebufferDimensions(
	const uint32_t render_pass_id 
) const {
	return m_framebuffers.GetDimensions( render_pass_id );
}

std::vector<VkClearValue>& MicroVulkanBackend::GetFramebufferClearValues(
	const uint32_t render_pass_id 
) {
	return m_framebuffers.GetClearValues( render_pass_id );
}

const std::vector<MicroVulkanFrameTarget>& MicroVulkanBackend::GetFramebufferTargets(
	const uint32_t render_pass_id
) const {
	return m_framebuffers.GetTargets( render_pass_id );
}

const MicroVulkanFrameTarget& MicroVulkanBackend::GetFramebufferTarget(
	const uint32_t render_pass_id,
	const uint32_t frame_id
) const {
	return m_framebuffers.GetTarget( render_pass_id, frame_id );
}

const MicroVulkanPipelines& MicroVulkanBackend::GetPipelines( ) const {
	return m_pipeline_cache;
}

const VkPipelineCache& MicroVulkanBackend::GetPipelineCache( ) const {
	return m_pipeline_cache.GetCache( );
}
