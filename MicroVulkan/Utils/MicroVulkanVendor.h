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

#include "MicroCore.h"

#define VK_UNUSED_FLAG 0

/**
 * vk namespace
 * @note : Used to store Vulkan wrapper function and extended types.
 **/
namespace vk {

	/**
	 * IsVkObject concept
	 * @note : Concept used to verify id an object is a vulkan object.
	 * @template VkType : Object type.
	 **/
	template<typename VkType>
	concept IsVkObject = std::is_pointer<VkType>::value;

	/**
	 * QueueTypes enum
	 * @note : Defined all wrapper supported device queue types.
	 **/
	enum QueueTypes : uint32_t {

		QUEUE_TYPE_GRAPHICS = 0,
		QUEUE_TYPE_TRANSFERT,
		QUEUE_TYPE_COMPUTE,

		QUEUE_TYPE_COUNT

	};

	/**
	 * QueueSpecification struct
	 * @note : Device queue specification.
	 **/
	struct QueueSpecification {

		uint32_t Graphics		= UINT32_MAX;
		uint32_t GraphicsCount  = 0;
		uint32_t Transfert		= UINT32_MAX;
		uint32_t TransfertCount = 0;
		uint32_t Compute		= UINT32_MAX;
		uint32_t ComputeCount	= 0;
		size_t Count			= 0;

		/**
		 * Cast operator
		 * @note : Cast specification to an unsigned integer array.
		 * @return : Address of the first element aka Graphics family index.
		 **/
		operator const uint32_t* ( ) const;

	};

	/**
	 * DeviceSpecification sturct
	 * @note : Device specification wrapper.
	 **/
	struct DeviceSpecification {

		VkPhysicalDeviceProperties Properties{ };
		VkPhysicalDeviceFeatures Features{ };
		QueueSpecification Queues{ };

	};

	/**
	 * PipelineBarrier struct
	 * @note : Defined Vulkan pipeline barrier base data wrapper.
	 **/
	struct PipelineBarrier {

		VkPipelineStageFlags SrcStageMask = VK_PIPELINE_STAGE_NONE;
		VkPipelineStageFlags DstStageMask = VK_PIPELINE_STAGE_NONE;
		VkDependencyFlags DependencyFlags = VK_UNUSED_FLAG;

	};

	/**
	 * BufferBindSpecification struct
	 * @note : Vulkan buffer bind specifiation wrapper.
	 **/
	struct BufferBindSpecification {

		VkBuffer Buffer		= VK_NULL_HANDLE;
		VkDeviceSize Offset = 0;

	};

	/**
	 * SetAllocationCallback method
	 * @note : Set current allocator callback structure.
	 * @param allocator : Reference to the new allocator callback structure.
	 **/
	void SetAllocationCallback( const VkAllocationCallbacks& allocator );

	/**
	 * GetAllocationCallback function
	 * @note : Get current allocator callback structure.
	 * @return : Reference to current allocator callback structure.
	 **/
	const VkAllocationCallbacks& GetAllocationCallback( );

	/**
	 * CreateInstance function
	 * @note : Wrapper for Vulkan instance creation.
	 * @param specification : Reference to instance speficiation.
	 * @param instance : Reference to Vulkan instance variable.
	 * @return : VkResult value of the call.
	 **/
	VkResult CreateInstance(
		const VkInstanceCreateInfo& specification,
		VkInstance& instance
	);

	/**
	 * DestroyInstance method
	 * @note : Wrapper for Vulkan instance destruction
	 * @param instance : Reference to Vulkan instance variable.
	 **/
	void DestroyInstance( VkInstance& instance );

	VkResult CreateDebugMessenger(
		const VkInstance& instance,
		const VkDebugUtilsMessengerCreateInfoEXT& specification,
		VkDebugUtilsMessengerEXT& messenger
	);

	void DestroyDebugMessenger(
		const VkInstance& instance,
		VkDebugUtilsMessengerEXT& messenger
	);

	void DestroySurface( VkInstance& instance, VkSurfaceKHR& surface );

	VkResult CreateDevice(
		const VkPhysicalDevice physical,
		const VkDeviceCreateInfo& specification,
		VkDevice& device
	);

	void DeviceWait( const VkDevice& device );

	void DestroyDevice( VkDevice& device );

	VkResult CreateSwapchain(
		const VkDevice& device,
		const VkSwapchainCreateInfoKHR& specification,
		VkSwapchainKHR& swapchain
	);

	void DestroySwapchain( const VkDevice& device, VkSwapchainKHR& swapchain );

	VkResult CreateRenderPass(
		const VkDevice& device,
		const VkRenderPassCreateInfo& specification,
		VkRenderPass& render_pass
	);

	void DestroyRenderPass( const VkDevice& device, VkRenderPass& render_pass );

	VkResult AllocateMemory(
		const VkDevice& device,
		const VkMemoryAllocateInfo& specification,
		VkDeviceMemory& memory
	);

	void DeallocateMemory( const VkDevice& device, VkDeviceMemory& memory );

	VkResult CreateImage(
		const VkDevice& device,
		const VkImageCreateInfo& specification,
		VkImage& image
	);

	void DestroyImage( const VkDevice& device, VkImage& image );

	VkResult CreateImageView(
		const VkDevice& device,
		const VkImageViewCreateInfo& specification,
		VkImageView& image_view
	);

	void DestroyImageView( const VkDevice& device, VkImageView& image_view );

	VkResult CreateImageSampler(
		const VkDevice& device,
		const VkSamplerCreateInfo& specification,
		VkSampler& sampler
	);

	void DestroyImageSampler( const VkDevice& device, VkSampler& sampler );

	VkResult CreateFramebuffer(
		const VkDevice& device,
		const VkFramebufferCreateInfo& specification,
		VkFramebuffer& framebuffer
	);

	void DestroyFramebuffer( const VkDevice& device, VkFramebuffer& framebuffer );

	VkResult CreateSemaphore(
		const VkDevice& device,
		const VkSemaphoreCreateInfo& specification,
		VkSemaphore& semaphore
	);

	void DestroySemaphore( const VkDevice& device, VkSemaphore& semaphore );

	VkResult CreateFence(
		const VkDevice& device,
		const VkFenceCreateInfo& specification,
		VkFence& fence
	);

	void DestroyFence( const VkDevice& device, VkFence& fence );

	VkResult CreateCommandPool(
		const VkDevice& device,
		const VkCommandPoolCreateInfo& specification,
		VkCommandPool& pool
	);

	void DestroyCommandPool( const VkDevice& device, VkCommandPool& pool );

	VkResult CreateBuffer( 
		const VkDevice& device,
		const VkBufferCreateInfo& specification,
		VkBuffer& buffer
	);

	void DestroyBuffer( const VkDevice& device, VkBuffer& buffer );

	VkResult CreatePipelineCache(
		const VkDevice& device,
		const VkPipelineCacheCreateInfo& specification,
		VkPipelineCache& pipeline_cache
	);

	void DestroyPipelineCache( const VkDevice& device, VkPipelineCache& pipeline_cache );

	VkResult CreatePipelineLayout(
		const VkDevice& device,
		const VkPipelineLayoutCreateInfo& specification,
		VkPipelineLayout& layout
	);

	void DestroyPipelineLayout( const VkDevice& device, VkPipelineLayout& layout );

	VkResult CreateDescriptorPool( 
		const VkDevice& device,
		const VkDescriptorPoolCreateInfo& specification,
		VkDescriptorPool& descriptor_pool
	);

	void DestroyDescriptorPool( const VkDevice& device, VkDescriptorPool& descriptor_pool );

	VkResult AllocateDescriptors(
		const VkDevice& device,
		const VkDescriptorSetAllocateInfo& specification,
		std::vector<VkDescriptorSet>& descriptor_list
	);

	void UpdateDescriptorSets(
		const VkDevice& device,
		const std::vector<VkWriteDescriptorSet>& writes,
		const std::vector<VkCopyDescriptorSet>& copies
	);

	void DeallocateDescriptors(
		const VkDevice& device, 
		const VkDescriptorPool& descriptor_pool, 
		std::vector<VkDescriptorSet>& descriptor_list 
	);

	VkResult CreateShader(
		const VkDevice& device,
		const VkShaderModuleCreateInfo& specification,
		VkShaderModule& shader
	);

	void DestroyShader( const VkDevice& device, VkShaderModule& shader );

	VkResult CreatePipeline(
		const VkDevice& device,
		const VkPipelineCache& cache,
		const VkGraphicsPipelineCreateInfo& specification,
		VkPipeline& pipeline
	);

	VkResult CreatePipeline(
		const VkDevice& device,
		const VkPipelineCache& cache,
		const VkComputePipelineCreateInfo& specification,
		VkPipeline& pipeline
	);

	void DestroyPipeline( const VkDevice& device, VkPipeline& pipeline );

	VkResult CreateQueryPool(
		const VkDevice& device,
		const VkQueryPoolCreateInfo& specification,
		VkQueryPool& query_pool
	);

	void DestroyQueryPool( const VkDevice& device, VkQueryPool& query_pool );

	VkResult WaitForFences( 
		const VkDevice& device,
		const VkFence& fence,
		const uint64_t wait_time
	);

	void CmdImageBarrier(
		const VkCommandBuffer& commands,
		const PipelineBarrier& specification,
		const VkImageMemoryBarrier& barrier
	);

	void CmdImageBarrier(
		const VkCommandBuffer& commands,
		const PipelineBarrier& specification,
		const std::vector<VkImageMemoryBarrier>& barriers
	);

	void CmdBufferBarrier(
		const VkCommandBuffer& commands,
		const PipelineBarrier& specification,
		const VkBufferMemoryBarrier& barrier
	);

	void CmdBufferBarrier(
		const VkCommandBuffer& commands,
		const PipelineBarrier& specification,
		const std::vector<VkBufferMemoryBarrier>& barriers
	);

	void CmdSetViewport(
		const VkCommandBuffer& commands,
		const VkViewport& viewport
	);

	void CmdSetViewport(
		const VkCommandBuffer& commands,
		const uint32_t start_id,
		const std::vector<VkViewport>& viewports
	);

	void CmdSetScissor(
		const VkCommandBuffer& commands,
		const VkRect2D& scissor
	);

	void CmdSetScissor( 
		const VkCommandBuffer& commands,
		const uint32_t start_id,
		const std::vector<VkRect2D>& scissors
	);

	void CmdBindDescriptorSets(
		const VkCommandBuffer& commands,
		const VkPipelineBindPoint bind_point,
		const VkPipelineLayout& layout,
		const uint32_t start_id,
		const std::vector<VkDescriptorSet>& descriptor_sets
	);

	void CmdBindDescriptorSets(
		const VkCommandBuffer& commands,
		const VkPipelineBindPoint bind_point,
		const VkPipelineLayout& layout,
		const uint32_t start_id,
		const std::vector<VkDescriptorSet>& descriptor_sets,
		const std::vector<uint32_t> dymanic_offsets
	);

	void CmdBindVertexBuffers( 
		const VkCommandBuffer& commands,
		const uint32_t start_id,
		const std::vector<BufferBindSpecification>& buffer_binds
	);

	void EnumerateInstanceExtension( std::vector<VkExtensionProperties>& extension_list );

	void EnumeratePhysicalDevices( const VkInstance& instance, std::vector<VkPhysicalDevice>& physical_list );

	void EnumeratePhysicalExtension( const VkPhysicalDevice& physical, std::vector<VkExtensionProperties>& extensions );

	void EnumeratePhysicalQueueFamilies( const VkPhysicalDevice& physical, std::vector<VkQueueFamilyProperties>& queue_list );

	/**
	 * EnumerateSwapchainFormats method
	 * @note : Wrapper for Vulkan surface format enumeration.
	 * @param physical : Reference to current Vulkan physical device instance.
	 * @param surface : Reference to current Vulkan surface instance.
	 * @param format_list : Reference to surface format vector.
	 **/
	void EnumerateSwapchainFormats(
		const VkPhysicalDevice& physical,
		const VkSurfaceKHR& surface,
		std::vector<VkSurfaceFormatKHR>& format_list
	);

	/**
	 * EnumerateSwapchainPresentModes method
	 * @note : Wrapper for Vulkan present mode enumeration.
	 * @param physical : Reference to current Vulkan physical device instance.
	 * @param surface : Reference to current Vulkan surface instance.
	 * @param present_modes : Reference to Vukan present mode vector.
	 **/
	void EnumerateSwapchainPresentModes(
		const VkPhysicalDevice& physical,
		const VkSurfaceKHR& surface,
		std::vector<VkPresentModeKHR>& present_modes
	);

	/**
	 * EnumerateSwapchainImages method
	 * @note : Wrapper for Vulkan swapchain image enumeration.
	 * @param device : Reference to current Vulkan device instance.
	 * @param swapchain : Reference to current Vulkan swapchain instance.
	 * @param image_list : Reference to swapchain image vector.
	 **/
	void EnumerateSwapchainImages(
		const VkDevice& device,
		const VkSwapchainKHR& swapchain,
		std::vector<VkImage>& image_list
	);

	/**
	 * GetPhysicalSpecification method
	 * @note : Wrapper for Vulkan device specification extraction
	 * @param physical : Reference to current Vulkan physical device instance.
	 * @param surface : Reference to current Vulkan surface instance.
	 * @param specification : Reference to physical device variable. 
	 **/
	void GetPhysicalSpecification( 
		const VkPhysicalDevice& physical,
		const VkSurfaceKHR& surface,
		DeviceSpecification& specification 
	);

	/**
	 * GetPipelineCacheData function
	 * @note : Wrapper for Vulkan pipeline cache extraction.
	 * @param device : Reference to current Vulkan device instance.
	 * @param pipeline_cache : Reference to current Vulkan pipeline cache instance.
	 * @param cache_data : Reference to pipeline cache byte vector.
	 * @return : VkResult value of the call.
	 **/
	VkResult GetPipelineCacheData(
		const VkDevice& device,
		const VkPipelineCache& pipeline_cache,
		std::vector<uint8_t>& cache_data
	);

	template<typename VkObject>
		requires IsVkObject<VkObject>
	bool IsValid( const VkObject& object ) {
		return object != VK_NULL_HANDLE;
	};

	template<typename VkFunction>
		requires IsVkObject<VkFunction>
	VkFunction GetInstanceProcAddr( const VkInstance& instance, const char* procedure ) {
		return (VkFunction)vkGetInstanceProcAddr( instance, procedure );
	};

};
