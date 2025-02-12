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
//		===	INTERNAL ===
////////////////////////////////////////////////////////////////////////////////////////////
#define vk_case_as_string( VALUE )\
	case VALUE : result_string = micro_stringify( VALUE ); break

VkAllocationCallbacks* ivk_Allocator = VK_NULL_HANDLE;
PFN_vkCreateDebugUtilsMessengerEXT ivk_CreateDebugMessenger = VK_NULL_HANDLE;
PFN_vkDestroyDebugUtilsMessengerEXT ivk_DestroyDebugMessenger = VK_NULL_HANDLE;

////////////////////////////////////////////////////////////////////////////////////////////
//		===	PUBLIC ===
////////////////////////////////////////////////////////////////////////////////////////////
namespace vk {

	QueueSpecification::operator const uint32_t* ( ) const {
		return micro_ptr( Graphics );
	}

	void SetAllocationCallback( const VkAllocationCallbacks& allocator ) {
		ivk_Allocator = micro_ptr_as( allocator, VkAllocationCallbacks* );
	}

	const VkAllocationCallbacks& GetAllocationCallback( ) {
		return micro_ref( ivk_Allocator );
	}

	bool GetIsSuccess( const VkResult result ) {
		auto state = true;

		switch ( result ) {
			case  VK_ERROR_OUT_OF_HOST_MEMORY:
			case  VK_ERROR_OUT_OF_DEVICE_MEMORY:
			case  VK_ERROR_INITIALIZATION_FAILED:
			case  VK_ERROR_DEVICE_LOST:
			case  VK_ERROR_MEMORY_MAP_FAILED:
			case  VK_ERROR_LAYER_NOT_PRESENT:
			case  VK_ERROR_EXTENSION_NOT_PRESENT:
			case  VK_ERROR_FEATURE_NOT_PRESENT:
			case  VK_ERROR_INCOMPATIBLE_DRIVER:
			case  VK_ERROR_TOO_MANY_OBJECTS:
			case  VK_ERROR_FORMAT_NOT_SUPPORTED:
			case  VK_ERROR_FRAGMENTED_POOL:
			case  VK_ERROR_SURFACE_LOST_KHR:
			case  VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
			case  VK_ERROR_OUT_OF_DATE_KHR:
			case  VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
			case  VK_ERROR_INVALID_SHADER_NV:
			case  VK_ERROR_OUT_OF_POOL_MEMORY:
			case  VK_ERROR_INVALID_EXTERNAL_HANDLE:
			case  VK_ERROR_FRAGMENTATION:
			case  VK_ERROR_INVALID_DEVICE_ADDRESS_EXT:
			case  VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT:
			case  VK_ERROR_UNKNOWN:
				state = false;
				break;

			default : break;
		}

		return state;
	}

	micro_string GetResultString( const VkResult result ) {
		auto result_string = "";

		switch ( result ) {
			vk_case_as_string( VK_SUCCESS );
			vk_case_as_string( VK_NOT_READY );
			vk_case_as_string( VK_TIMEOUT );
			vk_case_as_string( VK_EVENT_SET );
			vk_case_as_string( VK_EVENT_RESET );
			vk_case_as_string( VK_INCOMPLETE );
			vk_case_as_string( VK_ERROR_OUT_OF_HOST_MEMORY );
			vk_case_as_string( VK_ERROR_OUT_OF_DEVICE_MEMORY );
			vk_case_as_string( VK_ERROR_INITIALIZATION_FAILED );
			vk_case_as_string( VK_ERROR_DEVICE_LOST );
			vk_case_as_string( VK_ERROR_MEMORY_MAP_FAILED );
			vk_case_as_string( VK_ERROR_LAYER_NOT_PRESENT );
			vk_case_as_string( VK_ERROR_EXTENSION_NOT_PRESENT );
			vk_case_as_string( VK_ERROR_FEATURE_NOT_PRESENT );
			vk_case_as_string( VK_ERROR_INCOMPATIBLE_DRIVER );
			vk_case_as_string( VK_ERROR_TOO_MANY_OBJECTS );
			vk_case_as_string( VK_ERROR_FORMAT_NOT_SUPPORTED );
			vk_case_as_string( VK_ERROR_FRAGMENTED_POOL );
			vk_case_as_string( VK_ERROR_UNKNOWN );
			vk_case_as_string( VK_ERROR_OUT_OF_POOL_MEMORY );
			vk_case_as_string( VK_ERROR_INVALID_EXTERNAL_HANDLE );
			vk_case_as_string( VK_ERROR_FRAGMENTATION );
			vk_case_as_string( VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS );
			vk_case_as_string( VK_PIPELINE_COMPILE_REQUIRED );
			vk_case_as_string( VK_ERROR_SURFACE_LOST_KHR );
			vk_case_as_string( VK_ERROR_NATIVE_WINDOW_IN_USE_KHR );
			vk_case_as_string( VK_SUBOPTIMAL_KHR );
			vk_case_as_string( VK_ERROR_OUT_OF_DATE_KHR );
			vk_case_as_string( VK_ERROR_INCOMPATIBLE_DISPLAY_KHR );
			vk_case_as_string( VK_ERROR_VALIDATION_FAILED_EXT );
			vk_case_as_string( VK_ERROR_INVALID_SHADER_NV );
			vk_case_as_string( VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR );
			vk_case_as_string( VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR );
			vk_case_as_string( VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR );
			vk_case_as_string( VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR );
			vk_case_as_string( VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR );
			vk_case_as_string( VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR );
			vk_case_as_string( VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT );
			vk_case_as_string( VK_ERROR_NOT_PERMITTED_KHR );
			vk_case_as_string( VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT );
			vk_case_as_string( VK_THREAD_IDLE_KHR );
			vk_case_as_string( VK_THREAD_DONE_KHR );
			vk_case_as_string( VK_OPERATION_DEFERRED_KHR );
			vk_case_as_string( VK_OPERATION_NOT_DEFERRED_KHR );
			vk_case_as_string( VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR );
			vk_case_as_string( VK_ERROR_COMPRESSION_EXHAUSTED_EXT );
			vk_case_as_string( VK_INCOMPATIBLE_SHADER_BINARY_EXT );
			vk_case_as_string( VK_PIPELINE_BINARY_MISSING_KHR );
			vk_case_as_string( VK_ERROR_NOT_ENOUGH_SPACE_KHR );
			vk_case_as_string( VK_RESULT_MAX_ENUM );

			default : break;
		}

		return result_string;
	}

	VkResult CreateInstance(
		const VkInstanceCreateInfo& specification,
		VkInstance& instance
	) {
		return vkCreateInstance( micro_ptr( specification ), ivk_Allocator, micro_ptr( instance ) );
	}

	void DestroyInstance( VkInstance& instance ) {
		if ( !IsValid( instance ) )
			return;

		vkDestroyInstance( instance, ivk_Allocator );

		instance = VK_NULL_HANDLE;
	}

	VkResult CreateDebugMessenger(
		const VkInstance& instance,
		const VkDebugUtilsMessengerCreateInfoEXT& specification,
		VkDebugUtilsMessengerEXT& messenger
	) {
		micro_assert( IsValid( instance ), "Vulkan Instance must be valid to use this function" );

		ivk_CreateDebugMessenger = vk::GetInstanceProcAddr<PFN_vkCreateDebugUtilsMessengerEXT>( instance, "vkCreateDebugUtilsMessengerEXT" );
		ivk_DestroyDebugMessenger = vk::GetInstanceProcAddr<PFN_vkDestroyDebugUtilsMessengerEXT>( instance, "vkDestroyDebugUtilsMessengerEXT" );

		return ivk_CreateDebugMessenger( instance, micro_ptr( specification ), ivk_Allocator, micro_ptr( messenger ) );
	}

	void DeviceWait( const VkDevice& device ) {
		micro_assert( IsValid( device ), "Vulkan Device must be valid to use this function" );

		vkDeviceWaitIdle( device );
	}

	void DestroyDebugMessenger(
		const VkInstance& instance,
		VkDebugUtilsMessengerEXT& messenger
	) {
		micro_assert( IsValid( instance ), "Vulkan Instance must be valid to use this function" );

		if ( !vk::IsValid( messenger ) )
			return;

		ivk_DestroyDebugMessenger( instance, messenger, ivk_Allocator );

		messenger = nullptr;
	}

	void DestroySurface( VkInstance& instance, VkSurfaceKHR& surface ) {
		if ( !IsValid( instance ) || !IsValid( surface ) )
			return;

		vkDestroySurfaceKHR( instance, surface, ivk_Allocator );

		surface = VK_NULL_HANDLE;
	}

	VkResult CreateDevice(
		const VkPhysicalDevice physical,
		const VkDeviceCreateInfo& specification,
		VkDevice& device
	) {
		micro_assert( IsValid( physical ), "Vulkan Physical Device must be valid to use this function" );

		return vkCreateDevice( physical, micro_ptr( specification ), ivk_Allocator, micro_ptr( device ) );
	}

	void DestroyDevice( VkDevice& device ) {
		if ( !IsValid( device ) )
			return;

		vkDestroyDevice( device, ivk_Allocator );

		device = nullptr;
	}

	VkResult CreateSwapchain(
		const VkDevice& device,
		const VkSwapchainCreateInfoKHR& specification,
		VkSwapchainKHR& swapchain
	) {
		micro_assert( IsValid( device ), "Vulkan Device must be valid to use this function" );

		return vkCreateSwapchainKHR( device, micro_ptr( specification ), ivk_Allocator, micro_ptr( swapchain ) );
	}

	void DestroySwapchain( const VkDevice& device, VkSwapchainKHR& swapchain ) {
		if ( !IsValid( device ) || !IsValid( swapchain ) )
			return;

		vkDestroySwapchainKHR( device, swapchain, ivk_Allocator );

		swapchain = VK_NULL_HANDLE;
	}

	VkResult CreateRenderPass(
		const VkDevice& device,
		const VkRenderPassCreateInfo& specification,
		VkRenderPass& render_pass
	) {
		micro_assert( IsValid( device ), "Vulkan Device must be valid to use this function" );

		return vkCreateRenderPass( device, micro_ptr( specification ), ivk_Allocator, micro_ptr( render_pass ) );
	}

	void DestroyRenderPass( const VkDevice& device, VkRenderPass& render_pass ) {
		if ( !IsValid( device ) || !IsValid( render_pass ) )
			return;

		vkDestroyRenderPass( device, render_pass, ivk_Allocator );

		render_pass = VK_NULL_HANDLE;
	}

	VkResult AllocateMemory(
		const VkDevice& device,
		const VkMemoryAllocateInfo& specification,
		VkDeviceMemory& memory
	) {
		micro_assert( IsValid( device ), "Vulkan Device must be valid to use this function" );

		return vkAllocateMemory( device, micro_ptr( specification ), ivk_Allocator, micro_ptr( memory ) );
	}

	void DeallocateMemory( const VkDevice& device, VkDeviceMemory& memory ) {
		if ( !IsValid( device ) || !IsValid( memory ) )
			return;

		vkFreeMemory( device, memory, ivk_Allocator );

		memory = VK_NULL_HANDLE;
	}

	VkResult CreateImage(
		const VkDevice& device,
		const VkImageCreateInfo& specification,
		VkImage& image
	) {
		micro_assert( IsValid( device ), "Vulkan Device must be valid to use this function" );

		return vkCreateImage( device, micro_ptr( specification ), ivk_Allocator, micro_ptr( image ) );
	}

	void DestroyImage( const VkDevice& device, VkImage& image ) {
		if ( !IsValid( device ) || !IsValid( image ) )
			return;

		vkDestroyImage( device, image, ivk_Allocator );

		image = VK_NULL_HANDLE;
	}

	VkResult CreateImageView(
		const VkDevice& device,
		const VkImageViewCreateInfo& specification,
		VkImageView& image_view
	) {
		micro_assert( IsValid( device ), "Vulkan Device must be valid to use this function" );

		return vkCreateImageView( device, micro_ptr( specification ), ivk_Allocator, micro_ptr( image_view ) );
	}

	void DestroyImageView( const VkDevice& device, VkImageView& image_view ) {
		if ( !IsValid( device ) || !IsValid( image_view ) )
			return;

		vkDestroyImageView( device, image_view, ivk_Allocator );

		image_view = VK_NULL_HANDLE;
	}

	VkResult CreateImageSampler(
		const VkDevice& device,
		const VkSamplerCreateInfo& specification,
		VkSampler& sampler
	) {
		micro_assert( IsValid( device ), "Vulkan Device must be valid to use this function" );

		return vkCreateSampler( device, micro_ptr( specification ), ivk_Allocator, micro_ptr( sampler ) );
	}

	void DestroyImageSampler( const VkDevice& device, VkSampler& sampler ) {
		if ( !IsValid( device ) || !IsValid( sampler ) )
			return;

		vkDestroySampler( device, sampler, ivk_Allocator );

		sampler = VK_NULL_HANDLE;
	}

	VkResult CreateFramebuffer(
		const VkDevice& device,
		const VkFramebufferCreateInfo& specification,
		VkFramebuffer& framebuffer
	) {
		micro_assert( IsValid( device ), "Vulkan Device must be valid to use this function" );

		return vkCreateFramebuffer( device, micro_ptr( specification ), ivk_Allocator, micro_ptr( framebuffer ) );
	}

	void DestroyFramebuffer( const VkDevice& device, VkFramebuffer& framebuffer ) {
		if ( !IsValid( device ) || !IsValid( framebuffer ) )
			return;

		vkDestroyFramebuffer( device, framebuffer, ivk_Allocator );

		framebuffer = VK_NULL_HANDLE;
	}

	VkResult CreateSemaphore(
		const VkDevice& device,
		const VkSemaphoreCreateInfo& specification,
		VkSemaphore& semaphore
	) {
		micro_assert( IsValid( device ), "Vulkan Device must be valid to use this function" );

		return vkCreateSemaphore( device, micro_ptr( specification ), ivk_Allocator, micro_ptr( semaphore ) );
	}

	void DestroySemaphore( const VkDevice& device, VkSemaphore& semaphore ) {
		if ( !IsValid( device ) || !IsValid( semaphore ) )
			return;

		vkDestroySemaphore( device, semaphore, ivk_Allocator );

		semaphore = VK_NULL_HANDLE;
	}

	VkResult CreateFence(
		const VkDevice& device,
		const VkFenceCreateInfo& specification,
		VkFence& fence
	) {
		micro_assert( IsValid( device ), "Vulkan Device must be valid to use this function" );

		return vkCreateFence( device, micro_ptr( specification ), ivk_Allocator, micro_ptr( fence ) );
	}

	void DestroyFence( const VkDevice& device, VkFence& fence ) {
		if ( !IsValid( device ) || !IsValid( fence ) )
			return;

		vkDestroyFence( device, fence, ivk_Allocator );

		fence = VK_NULL_HANDLE;
	}

	VkResult CreateCommandPool(
		const VkDevice& device,
		const VkCommandPoolCreateInfo& specification,
		VkCommandPool& pool
	) {
		micro_assert( IsValid( device ), "Vulkan Device must be valid to use this function" );

		return vkCreateCommandPool( device, micro_ptr( specification ), ivk_Allocator, micro_ptr( pool ) );
	}

	void DestroyCommandPool( const VkDevice& device, VkCommandPool& pool ) {
		if ( !IsValid( device ) || !IsValid( pool ) )
			return;

		vkDestroyCommandPool( device, pool, ivk_Allocator );

		pool = VK_NULL_HANDLE;
	}

	VkResult CreateBuffer(
		const VkDevice& device,
		const VkBufferCreateInfo& specification,
		VkBuffer& buffer
	) {
		micro_assert( IsValid( device ), "Vulkan Device must be valid to use this function" );

		return vkCreateBuffer( device, micro_ptr( specification ), ivk_Allocator, micro_ptr( buffer ) );
	}

	void DestroyBuffer( const VkDevice& device, VkBuffer& buffer ) {
		if ( !IsValid( device ) || !IsValid( buffer ) )
			return;

		vkDestroyBuffer( device, buffer, ivk_Allocator );

		buffer = VK_NULL_HANDLE;
	}

	VkResult CreatePipelineCache(
		const VkDevice& device,
		const VkPipelineCacheCreateInfo& specification,
		VkPipelineCache& pipeline_cache
	) {
		micro_assert( IsValid( device ), "Vulkan Device must be valid to use this function" );

		return vkCreatePipelineCache( device, micro_ptr( specification ), ivk_Allocator, micro_ptr( pipeline_cache ) );
	}

	void DestroyPipelineCache( const VkDevice& device, VkPipelineCache& pipeline_cache ) {
		if ( !IsValid( device ) || !IsValid( pipeline_cache ) )
			return;

		vkDestroyPipelineCache( device, pipeline_cache, ivk_Allocator );

		pipeline_cache = VK_NULL_HANDLE;
	}

	VkResult CreatePipelineLayout(
		const VkDevice& device,
		const VkPipelineLayoutCreateInfo& specification,
		VkPipelineLayout& layout
	) {
		micro_assert( IsValid( device ), "Vulkan Device must be valid to use this function" );

		return vkCreatePipelineLayout( device, micro_ptr( specification ), ivk_Allocator, micro_ptr( layout ) );
	}

	void DestroyPipelineLayout( const VkDevice& device, VkPipelineLayout& layout ) {
		if ( !IsValid( device ) || !IsValid( layout ) )
			return;

		vkDestroyPipelineLayout( device, layout, ivk_Allocator );

		layout = VK_NULL_HANDLE;
	}

	VkResult CreateDescriptorPool(
		const VkDevice& device,
		const VkDescriptorPoolCreateInfo& specification,
		VkDescriptorPool& descriptor_pool
	) {
		micro_assert( IsValid( device ), "Vulkan Device must be valid to use this function" );

		return vkCreateDescriptorPool( device, micro_ptr( specification ), ivk_Allocator, micro_ptr( descriptor_pool ) );
	}

	void DestroyDescriptorPool( const VkDevice& device, VkDescriptorPool& descriptor_pool ) {
		if ( !IsValid( device ) || !IsValid( descriptor_pool ) )
			return;

		vkDestroyDescriptorPool( device, descriptor_pool, ivk_Allocator );

		descriptor_pool = VK_NULL_HANDLE;
	}

	VkResult AllocateDescriptors(
		const VkDevice& device,
		const VkDescriptorSetAllocateInfo& specification,
		std::vector<VkDescriptorSet>& descriptor_list
	) {
		micro_assert( IsValid( device ), "Vulkan Device must be valid to use this function" );

		descriptor_list.resize( (size_t)specification.descriptorSetCount );

		return vkAllocateDescriptorSets( device, micro_ptr( specification ), descriptor_list.data( ) );
	}

	void UpdateDescriptorSets(
		const VkDevice& device,
		const std::vector<VkWriteDescriptorSet>& writes,
		const std::vector<VkCopyDescriptorSet>& copies
	) {
		micro_assert( IsValid( device ), "Vulkan Device must be valid to use this function" );

		const auto* write_data = writes.data( );
		const auto* copy_data  = copies.data( );
		auto write_count	   = (uint32_t)writes.size( );
		auto copy_count		   = (uint32_t)copies.size( );

		vkUpdateDescriptorSets( device, write_count, write_data, copy_count, copy_data );
	}

	void DeallocateDescriptors(
		const VkDevice& device,
		const VkDescriptorPool& descriptor_pool,
		std::vector<VkDescriptorSet>& descriptor_list
	) {
		if ( !IsValid( device ) || !IsValid( descriptor_pool ) )
			return;

		const auto* list_data = descriptor_list.data( );
		auto descriptor_count = (uint32_t)descriptor_list.size( );

		if ( descriptor_count > 0 )
			vkFreeDescriptorSets( device, descriptor_pool, descriptor_count, list_data );
	}

	VkResult CreateShader(
		const VkDevice& device,
		const VkShaderModuleCreateInfo& specification,
		VkShaderModule& shader
	) {
		micro_assert( IsValid( device ), "Vulkan Device must be valid to use this function" );

		return vkCreateShaderModule( device, micro_ptr( specification ), ivk_Allocator, micro_ptr( shader ) );
	}

	void DestroyShader( const VkDevice& device, VkShaderModule& shader ) {
		if ( !IsValid( device ) || !IsValid( shader ) )
			return;

		vkDestroyShaderModule( device, shader, ivk_Allocator );

		shader = VK_NULL_HANDLE;
	}

	VkResult CreatePipeline(
		const VkDevice& device,
		const VkPipelineCache& cache,
		const VkGraphicsPipelineCreateInfo& specification,
		VkPipeline& pipeline
	) {
		micro_assert( IsValid( device ), "Vulkan Device must be valid to use this function" );

		return vkCreateGraphicsPipelines( device, cache, 1, micro_ptr( specification ), ivk_Allocator, micro_ptr( pipeline ) );
	}

	VkResult CreatePipeline(
		const VkDevice& device,
		const VkPipelineCache& cache,
		const VkComputePipelineCreateInfo& specification,
		VkPipeline& pipeline
	) {
		micro_assert( IsValid( device ), "Vulkan Device must be valid to use this function" );

		return vkCreateComputePipelines( device, cache, 1, micro_ptr( specification ), ivk_Allocator, micro_ptr( pipeline ) );
	}

	void DestroyPipeline( const VkDevice& device, VkPipeline& pipeline ) {
		if ( !IsValid( device ) || !IsValid( pipeline ) )
			return;

		vkDestroyPipeline( device, pipeline, ivk_Allocator );

		pipeline = VK_NULL_HANDLE;
	}

	VkResult CreateQueryPool(
		const VkDevice& device,
		const VkQueryPoolCreateInfo& specification,
		VkQueryPool& query_pool
	) {
		micro_assert( IsValid( device ), "Vulkan Device must be valid to use this function" );

		return vkCreateQueryPool( device, micro_ptr( specification ), ivk_Allocator, micro_ptr( query_pool ) );
	}

	void DestroyQueryPool( const VkDevice& device, VkQueryPool& query_pool ) {
		if ( !IsValid( device ) || !IsValid( query_pool ) )
			return;

		vkDestroyQueryPool( device, query_pool, ivk_Allocator );

		query_pool = VK_NULL_HANDLE;
	}

	VkResult WaitForFences(
		const VkDevice& device,
		const VkFence& fence,
		const uint64_t wait_time
	) {
		micro_assert( IsValid( device ), "Vulkan Device must be valid to use this function" );
		micro_assert( IsValid( fence ), "Vulkan Fence must be valid to use this function" );

		return vkWaitForFences( device, 1, micro_ptr( fence ), VK_TRUE, wait_time );
	}

	void CmdImageBarrier(
		const VkCommandBuffer& commands,
		const PipelineBarrier& specification,
		const VkImageMemoryBarrier& barrier
	) {
		micro_assert( IsValid( commands ), "You can't execute command on an invalid command buffer" );

		if ( !IsValid( commands ) )
			return;

		vkCmdPipelineBarrier(
			commands,
			specification.SrcStageMask,
			specification.DstStageMask,
			specification.DependencyFlags,
			0, VK_NULL_HANDLE,
			0, VK_NULL_HANDLE,
			1, micro_ptr( barrier )
		);
	}

	void CmdImageBarrier(
		const VkCommandBuffer& commands,
		const PipelineBarrier& specification,
		const std::vector<VkImageMemoryBarrier>& barriers
	) {
		micro_assert( IsValid( commands ), "You can't execute command on an invalid command buffer" );

		if ( !IsValid( commands ) )
			return;

		auto barrier_count = (uint32_t)barriers.size( );

		if ( barrier_count > 0 ) {
			const auto* barrier_data = barriers.data( );

			vkCmdPipelineBarrier(
				commands,
				specification.SrcStageMask,
				specification.DstStageMask,
				specification.DependencyFlags,
				0, VK_NULL_HANDLE,
				0, VK_NULL_HANDLE,
				barrier_count, barrier_data
			);
		}
	}

	void CmdBufferBarrier(
		const VkCommandBuffer& commands,
		const PipelineBarrier& specification,
		const VkBufferMemoryBarrier& barrier
	) {
		micro_assert( IsValid( commands ), "You can't execute command on an invalid command buffer" );

		if ( !IsValid( commands ) )
			return;

		vkCmdPipelineBarrier(
			commands,
			specification.SrcStageMask,
			specification.DstStageMask,
			specification.DependencyFlags,
			0, VK_NULL_HANDLE,
			1, micro_ptr( barrier ),
			0, VK_NULL_HANDLE
		);
	}

	void CmdBufferBarrier(
		const VkCommandBuffer& commands,
		const PipelineBarrier& specification,
		const std::vector<VkBufferMemoryBarrier>& barriers
	) {
		micro_assert( IsValid( commands ), "You can't execute command on an invalid command buffer" );

		if ( !IsValid( commands ) )
			return;
		
		auto barrier_count = (uint32_t)barriers.size( );

		if ( barrier_count > 0 ) {
			const auto* barrier_data = barriers.data( );

			vkCmdPipelineBarrier(
				commands,
				specification.SrcStageMask,
				specification.DstStageMask,
				specification.DependencyFlags,
				0, VK_NULL_HANDLE,
				barrier_count, barrier_data,
				0, VK_NULL_HANDLE
			);
		}
	}

	void CmdSetViewport(
		const VkCommandBuffer& commands,
		const VkViewport& viewport
	) {
		micro_assert( IsValid( commands ), "You can't execute command on an invalid command buffer" );

		vkCmdSetViewport( commands, 0, 1, micro_ptr( viewport ) );
	}

	void CmdSetViewport(
		const VkCommandBuffer& commands,
		const uint32_t start_id,
		const std::vector<VkViewport>& viewports
	) {
		micro_assert( IsValid( commands ), "You can't execute command on an invalid command buffer" );

		const auto* viewport_data = viewports.data( );
		auto viewport_count		  = (uint32_t)viewports.size( );

		if ( viewport_count > 0 )
			vkCmdSetViewport( commands, start_id, viewport_count, viewport_data );
	}

	void CmdSetScissor(
		const VkCommandBuffer& commands,
		const VkRect2D& scissor
	) {
		micro_assert( IsValid( commands ), "You can't execute command on an invalid command buffer" );

		vkCmdSetScissor( commands, 0, 1, micro_ptr( scissor ) );
	}

	void CmdSetScissor(
		const VkCommandBuffer& commands,
		const uint32_t start_id,
		const std::vector<VkRect2D>& scissors
	) {
		micro_assert( IsValid( commands ), "You can't execute command on an invalid command buffer" );

		const auto* scissor_data = scissors.data( );
		auto scissor_count = (uint32_t)scissors.size( );

		if ( scissor_count > 0 )
			vkCmdSetScissor( commands, start_id, scissor_count, scissor_data );
	}

	void CmdBindDescriptorSets(
		const VkCommandBuffer& commands,
		const VkPipelineBindPoint bind_point,
		const VkPipelineLayout& layout,
		const uint32_t start_id,
		const std::vector<VkDescriptorSet>& descriptor_sets
	) {
		micro_assert( IsValid( commands ), "You can't execute command on an invalid command buffer" );

		const auto* descriptor_data = descriptor_sets.data( );
		auto descriptor_count		= (uint32_t)descriptor_sets.size( );

		if ( descriptor_count > 0 )
			vkCmdBindDescriptorSets( commands, bind_point, layout, start_id, descriptor_count, descriptor_data, 0, VK_NULL_HANDLE );
	}

	void CmdBindDescriptorSets(
		const VkCommandBuffer& commands,
		const VkPipelineBindPoint bind_point,
		const VkPipelineLayout& layout,
		const uint32_t start_id,
		const std::vector<VkDescriptorSet>& descriptor_sets,
		const std::vector<uint32_t> dymanic_offsets
	) {
		micro_assert( IsValid( commands ), "You can't execute command on an invalid command buffer" );
		
		const auto* descriptor_data = descriptor_sets.data( );
		const auto* offset_data		= dymanic_offsets.data( );
		auto descriptor_count		= (uint32_t)descriptor_sets.size( );
		auto offset_count			= (uint32_t)dymanic_offsets.size( );

		if ( descriptor_count > 0 )
			vkCmdBindDescriptorSets( commands, bind_point, layout, start_id, descriptor_count, descriptor_data, offset_count, offset_data );
	}

	std::vector<VkBuffer> EnumerateBuffers( const std::vector<BufferBindSpecification>& buffer_binds ) {
		auto buffer_id = buffer_binds.size( );
		auto buffers   = std::vector<VkBuffer>{ };

		buffers.resize( buffer_id );

		while ( buffer_id-- > 0 ) {
			auto& buffer = buffer_binds[ buffer_id ].Buffer;

			if ( IsValid( buffer ) )
				buffers[ buffer_id ] = buffer;
		}

		return buffers;
	}

	std::vector<VkDeviceSize> EnumerateBufferOffsets( const std::vector<BufferBindSpecification>& buffer_binds ) {
		auto offset_id = buffer_binds.size( );
		auto offsets   = std::vector<VkDeviceSize>{ };

		offsets.resize( offset_id );

		while ( offset_id-- > 0 ) {
			auto& buffer = buffer_binds[ offset_id ];

			if ( IsValid( buffer.Buffer ) )
				offsets[ offset_id ] = buffer.Offset;
		}

		return offsets;
	}

	void CmdBindVertexBuffers(
		const VkCommandBuffer& commands,
		const uint32_t start_id,
		const std::vector<BufferBindSpecification>& buffer_binds
	) {
		micro_assert( IsValid( commands ), "You can't execute command on an invalid command buffer" );

		auto buffers			= EnumerateBuffers( buffer_binds );
		auto offsets			= EnumerateBufferOffsets( buffer_binds );
		auto buffer_count		= (uint32_t)buffers.size( );
		const auto* buffer_data = buffers.data( );
		const auto* offset_data = offsets.data( );

		if ( buffer_count > 0 )
			vkCmdBindVertexBuffers( commands, start_id, buffer_count, buffer_data, offset_data );
	}

	void EnumerateInstanceExtension( std::vector<VkExtensionProperties>& extension_list ) {
		auto list_size = (uint32_t)0;

		vkEnumerateInstanceExtensionProperties( VK_NULL_HANDLE, micro_ptr( list_size ), VK_NULL_HANDLE );

		extension_list.resize( list_size );

		auto* list_data = extension_list.data( );

		vkEnumerateInstanceExtensionProperties( VK_NULL_HANDLE, micro_ptr( list_size ), list_data );
	}

	void EnumeratePhysicalDevices( const VkInstance& instance, std::vector<VkPhysicalDevice>& physical_list ) {
		micro_assert( IsValid( instance ), "Vulkan Instance must be valid to use this function" );

		auto list_size = (uint32_t)0;

		vkEnumeratePhysicalDevices( instance, micro_ptr( list_size ), VK_NULL_HANDLE );

		physical_list.resize( list_size );

		auto* list_data = physical_list.data( );

		vkEnumeratePhysicalDevices( instance, micro_ptr( list_size ), list_data );
	}

	void EnumeratePhysicalExtension( const VkPhysicalDevice& physical, std::vector<VkExtensionProperties>& extension_list ) {
		micro_assert( IsValid( physical ), "Vulkan Physical Device must be valid to use this function" );

		auto list_size = (uint32_t)0;

		vkEnumerateDeviceExtensionProperties( physical, VK_NULL_HANDLE, micro_ptr( list_size ), VK_NULL_HANDLE );

		extension_list.resize( list_size );

		auto* list_data = extension_list.data( );

		vkEnumerateDeviceExtensionProperties( physical, VK_NULL_HANDLE, micro_ptr( list_size ), list_data );
	}

	void EnumeratePhysicalQueueFamilies( const VkPhysicalDevice& physical, std::vector<VkQueueFamilyProperties>& queue_list ) {
		micro_assert( IsValid( physical ), "Vulkan Physical Device must be valid to use this function" );

		auto list_size = (uint32_t)0;
		
		vkGetPhysicalDeviceQueueFamilyProperties( physical, micro_ptr( list_size ), VK_NULL_HANDLE );

		queue_list.resize( list_size );

		auto* list_data = queue_list.data( );

		vkGetPhysicalDeviceQueueFamilyProperties( physical, micro_ptr( list_size ), list_data );
	}

	void EnumerateSwapchainFormats(
		const VkPhysicalDevice& physical,
		const VkSurfaceKHR& surface,
		std::vector<VkSurfaceFormatKHR>& format_list
	) {
		micro_assert( IsValid( physical ), "Vulkan Physical Device must be valid to use this function" );
		micro_assert( IsValid( surface ), "Vulkan Surface must be valid to use this function" );

		auto list_size = (uint32_t)0;

		vkGetPhysicalDeviceSurfaceFormatsKHR( physical, surface, micro_ptr( list_size ), VK_NULL_HANDLE );

		format_list.resize( list_size );

		auto* list_data = format_list.data( );

		vkGetPhysicalDeviceSurfaceFormatsKHR( physical, surface, micro_ptr( list_size ), list_data );
	}

	void EnumerateSwapchainPresentModes( 
		const VkPhysicalDevice& physical,
		const VkSurfaceKHR& surface,
		std::vector<VkPresentModeKHR>& present_modes
	) {
		micro_assert( IsValid( physical ), "Vulkan Physical Device must be valid to use this function" );
		micro_assert( IsValid( surface ), "Vulkan Surface must be valid to use this function" );

		auto list_size = (uint32_t)0;

		vkGetPhysicalDeviceSurfacePresentModesKHR( physical, surface, micro_ptr( list_size ), VK_NULL_HANDLE );
		
		present_modes.resize( list_size );

		auto* list_data = present_modes.data( );

		vkGetPhysicalDeviceSurfacePresentModesKHR( physical, surface, micro_ptr( list_size ), list_data );
	}

	void EnumerateSwapchainImages(
		const VkDevice& device,
		const VkSwapchainKHR& swapchain,
		std::vector<VkImage>& image_list
	) {
		micro_assert( IsValid( device ), "Vulkan Device must be valid to use this function" );
		micro_assert( IsValid( swapchain ), "Vulkan Swapchain must be valid to use this function" );

		auto list_size = (uint32_t)0;

		vkGetSwapchainImagesKHR( device, swapchain, micro_ptr( list_size ), VK_NULL_HANDLE );

		image_list.resize( list_size );

		auto* list_data = image_list.data( );

		vkGetSwapchainImagesKHR( device, swapchain, micro_ptr( list_size ), list_data );
	}

	void GetGraphicQueueSpec(
		const VkPhysicalDevice& physical,
		const VkSurfaceKHR& surface,
		std::vector<VkQueueFamilyProperties>& families,
		QueueSpecification& specification
	) {
		auto has_present = VK_FALSE;
		auto family_id   = (uint32_t)0;

		for ( auto& family : families ) {
			vkGetPhysicalDeviceSurfaceSupportKHR( physical, family_id, surface, micro_ptr( has_present ) );

			if ( has_present && family.queueFlags & VK_QUEUE_GRAPHICS_BIT ) {
				specification.Graphics		= family_id;
				specification.GraphicsCount = family.queueCount;

				break;
			}
		}
	}

	void GetTransfertQueueSpec(
		std::vector<VkQueueFamilyProperties>& families,
		QueueSpecification& specification,
		bool need_compute = false
	) { 
		auto family_id = (uint32_t)0;

		for ( auto& family : families ) {
			if (
				 ( family.queueFlags & VK_QUEUE_TRANSFER_BIT		 ) &&
				!( family.queueFlags & VK_QUEUE_GRAPHICS_BIT		 ) &&
				!( family.queueFlags & VK_QUEUE_VIDEO_DECODE_BIT_KHR ) &&
				!( family.queueFlags & VK_QUEUE_VIDEO_ENCODE_BIT_KHR ) &&
				!( need_compute * ( family.queueFlags & VK_QUEUE_COMPUTE_BIT ) )
			)
				break;

			family_id += 1;
		}

		if ( family_id < (uint32_t)families.size( ) ) {
			specification.Transfert		 = family_id;
			specification.TransfertCount = families[ family_id ].queueCount;
		} else if ( !need_compute )
			GetTransfertQueueSpec( families, specification, true );
	}

	void GetComputeQueueSpec(
		std::vector<VkQueueFamilyProperties>& families,
		QueueSpecification& specification
	) { 
		auto family_id = (uint32_t)0;

		for ( auto& family : families ) {
			if (
				 ( family.queueFlags & VK_QUEUE_COMPUTE_BIT			 ) &&
				!( family.queueFlags & VK_QUEUE_GRAPHICS_BIT		 ) && 
				!( family.queueFlags & VK_QUEUE_VIDEO_DECODE_BIT_KHR ) &&
				!( family.queueFlags & VK_QUEUE_VIDEO_ENCODE_BIT_KHR )
			) {
				specification.Compute = family_id;
				specification.ComputeCount = families[ family_id ].queueCount;

				break;
			}

			family_id += 1;
		}
	}

	void GetFinalQueueSpec( QueueSpecification& specification ) {
		if ( specification.Compute == UINT32_MAX ) {
			if ( specification.Transfert < UINT32_MAX ) {
				specification.Compute	   = specification.Transfert;
				specification.ComputeCount = specification.TransfertCount / 3;

				specification.TransfertCount = 2 * specification.ComputeCount;
			} else {
				specification.Compute	   = specification.Graphics;
				specification.ComputeCount = specification.GraphicsCount / 4;

				specification.GraphicsCount -= specification.ComputeCount;
			}
		}
		
		if ( specification.Transfert == UINT32_MAX ) {
			specification.Transfert		 = specification.Graphics;
			specification.TransfertCount = specification.GraphicsCount / 4;

			specification.GraphicsCount -= specification.TransfertCount;
		}
	}

	void GetQueueSizeSpec( QueueSpecification& specification ) {
		specification.Count = specification.GraphicsCount;

		if ( specification.Count < specification.TransfertCount )
			specification.Count = specification.TransfertCount;

		if ( specification.Count < specification.ComputeCount )
			specification.Count = specification.ComputeCount;
	}

	void GetPhysicalQueueSpecification(
		const VkPhysicalDevice& physical,
		const VkSurfaceKHR& surface, 
		QueueSpecification& specification 
	) {
		auto families = std::vector<VkQueueFamilyProperties>{ };
		
		vk::EnumeratePhysicalQueueFamilies( physical, families );

		GetGraphicQueueSpec( physical, surface, families, specification );
		GetTransfertQueueSpec( families, specification );
		GetComputeQueueSpec( families, specification );
		GetFinalQueueSpec( specification );
		GetQueueSizeSpec( specification );
	}

	void GetPhysicalSpecification( 
		const VkPhysicalDevice& physical,
		const VkSurfaceKHR& surface,
		DeviceSpecification& specification
	) {
		micro_assert( IsValid( physical ), "Vulkan Physical Device must be valid to use this function" );
		micro_assert( IsValid( surface ), "Vulkan Surface must be valid to use this function" );

		vkGetPhysicalDeviceProperties( physical, micro_ptr( specification.Properties ) );
		vkGetPhysicalDeviceFeatures( physical, micro_ptr( specification.Features ) );

		GetPhysicalQueueSpecification( physical, surface, specification.Queues );
	}

	VkResult GetPipelineCacheData(
		const VkDevice& device,
		const VkPipelineCache& pipeline_cache,
		std::vector<uint8_t>& cache_data
	) {
		auto result = VK_ERROR_UNKNOWN;

		if ( IsValid( device ) && IsValid( pipeline_cache ) ) {
			auto cache_size = (size_t)0;

			vkGetPipelineCacheData( device, pipeline_cache, micro_ptr( cache_size ), VK_NULL_HANDLE );

			cache_data.resize( cache_size );

			auto* list_data = micro_cast( cache_data.data( ), void* );

			result = vkGetPipelineCacheData( device, pipeline_cache, micro_ptr( cache_size ), list_data );
		}

		return result;
	}

};

