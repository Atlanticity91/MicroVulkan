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
#define VK_UNDEFINED_STRING "VK_UNDEFINED"

typedef VkRect2D VkScissor;

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
	concept IsVkObject = std::is_pointer_v<VkType>;

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
	micro_struct QueueSpecification {

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
	micro_struct DeviceSpecification {

		VkPhysicalDeviceProperties Properties{ };
		VkPhysicalDeviceFeatures Features{ };
		QueueSpecification Queues{ };

	};

	/**
	 * PipelineBarrier struct
	 * @note : Defined Vulkan pipeline barrier base data wrapper.
	 **/
	micro_struct PipelineBarrier {

		VkPipelineStageFlags SrcStageMask = VK_PIPELINE_STAGE_NONE;
		VkPipelineStageFlags DstStageMask = VK_PIPELINE_STAGE_NONE;
		VkDependencyFlags DependencyFlags = VK_UNUSED_FLAG;

	};

	/**
	 * BufferBindSpecification struct
	 * @note : Vulkan buffer bind specifiation wrapper.
	 **/
	micro_struct BufferBindSpecification {

		VkBuffer Buffer		= VK_NULL_HANDLE;
		VkDeviceSize Offset = 0;

	};

	/**
	 * SetAllocationCallback method
	 * @note : Set current allocator callback structure.
	 * @param allocator : Reference to the new allocator callback structure.
	 **/
	MICRO_API void SetAllocationCallback( const VkAllocationCallbacks& allocator );

	/**
	 * GetAllocationCallback function
	 * @note : Get current allocator callback structure.
	 * @return : Reference to current allocator callback structure.
	 **/
	MICRO_API const VkAllocationCallbacks& GetAllocationCallback( );

	MICRO_API bool GetIsSuccess( const VkResult result );

	MICRO_API micro_string GetResultString( const VkResult result );

	/**
	 * CreateInstance function
	 * @note : Wrapper for Vulkan instance creation.
	 * @param specification : Reference to instance speficiation.
	 * @param instance : Reference to Vulkan instance variable.
	 * @return : VkResult value of the call.
	 **/
	MICRO_API VkResult CreateInstance(
		const VkInstanceCreateInfo& specification,
		VkInstance& instance
	);

	/**
	 * DestroyInstance method
	 * @note : Wrapper for Vulkan instance destruction
	 * @param instance : Reference to Vulkan instance variable.
	 **/
	MICRO_API void DestroyInstance( VkInstance& instance );

	MICRO_API VkResult CreateDebugMessenger(
		const VkInstance& instance,
		const VkDebugUtilsMessengerCreateInfoEXT& specification,
		VkDebugUtilsMessengerEXT& messenger
	);

	MICRO_API void DestroyDebugMessenger(
		const VkInstance& instance,
		VkDebugUtilsMessengerEXT& messenger
	);

	MICRO_API void DestroySurface( VkInstance& instance, VkSurfaceKHR& surface );

	MICRO_API VkResult CreateDevice(
		const VkPhysicalDevice physical,
		const VkDeviceCreateInfo& specification,
		VkDevice& device
	);

	MICRO_API void DeviceWait( const VkDevice& device );

	MICRO_API void DestroyDevice( VkDevice& device );

	MICRO_API VkResult CreateSwapchain(
		const VkDevice& device,
		const VkSwapchainCreateInfoKHR& specification,
		VkSwapchainKHR& swapchain
	);

	MICRO_API void DestroySwapchain( const VkDevice& device, VkSwapchainKHR& swapchain );

	MICRO_API VkResult CreateRenderPass(
		const VkDevice& device,
		const VkRenderPassCreateInfo& specification,
		VkRenderPass& render_pass
	);

	MICRO_API void DestroyRenderPass( const VkDevice& device, VkRenderPass& render_pass );

	MICRO_API VkResult AllocateMemory(
		const VkDevice& device,
		const VkMemoryAllocateInfo& specification,
		VkDeviceMemory& memory
	);

	MICRO_API void DeallocateMemory( const VkDevice& device, VkDeviceMemory& memory );

	MICRO_API VkResult CreateImage(
		const VkDevice& device,
		const VkImageCreateInfo& specification,
		VkImage& image
	);

	MICRO_API void DestroyImage( const VkDevice& device, VkImage& image );

	MICRO_API VkResult CreateImageView(
		const VkDevice& device,
		const VkImageViewCreateInfo& specification,
		VkImageView& image_view
	);

	MICRO_API void DestroyImageView( const VkDevice& device, VkImageView& image_view );

	MICRO_API VkResult CreateImageSampler(
		const VkDevice& device,
		const VkSamplerCreateInfo& specification,
		VkSampler& sampler
	);

	MICRO_API void DestroyImageSampler( const VkDevice& device, VkSampler& sampler );

	MICRO_API VkResult CreateFramebuffer(
		const VkDevice& device,
		const VkFramebufferCreateInfo& specification,
		VkFramebuffer& framebuffer
	);

	MICRO_API void DestroyFramebuffer( const VkDevice& device, VkFramebuffer& framebuffer );

	MICRO_API VkResult CreateSemaphore(
		const VkDevice& device,
		const VkSemaphoreCreateInfo& specification,
		VkSemaphore& semaphore
	);

	MICRO_API void DestroySemaphore( const VkDevice& device, VkSemaphore& semaphore );

	MICRO_API VkResult CreateFence(
		const VkDevice& device,
		const VkFenceCreateInfo& specification,
		VkFence& fence
	);

	MICRO_API void DestroyFence( const VkDevice& device, VkFence& fence );

	MICRO_API VkResult CreateCommandPool(
		const VkDevice& device,
		const VkCommandPoolCreateInfo& specification,
		VkCommandPool& pool
	);

	MICRO_API void DestroyCommandPool( const VkDevice& device, VkCommandPool& pool );

	MICRO_API VkResult CreateBuffer(
		const VkDevice& device,
		const VkBufferCreateInfo& specification,
		VkBuffer& buffer
	);

	MICRO_API void DestroyBuffer( const VkDevice& device, VkBuffer& buffer );

	MICRO_API VkResult CreatePipelineCache(
		const VkDevice& device,
		const VkPipelineCacheCreateInfo& specification,
		VkPipelineCache& pipeline_cache
	);

	MICRO_API void DestroyPipelineCache( const VkDevice& device,
							   VkPipelineCache& pipeline_cache );

	MICRO_API VkResult CreatePipelineLayout(
		const VkDevice& device,
		const VkPipelineLayoutCreateInfo& specification,
		VkPipelineLayout& layout
	);

	MICRO_API void DestroyPipelineLayout( const VkDevice& device, VkPipelineLayout& layout );

	MICRO_API VkResult CreateDescriptorPool(
		const VkDevice& device,
		const VkDescriptorPoolCreateInfo& specification,
		VkDescriptorPool& descriptor_pool
	);

	MICRO_API void DestroyDescriptorPool(
		const VkDevice& device, 
		VkDescriptorPool& descriptor_pool 
	);

	MICRO_API VkResult AllocateDescriptors(
		const VkDevice& device,
		const VkDescriptorSetAllocateInfo& specification,
		std::vector<VkDescriptorSet>& descriptor_list
	);

	MICRO_API void UpdateDescriptorSets(
		const VkDevice& device,
		const std::vector<VkWriteDescriptorSet>& writes,
		const std::vector<VkCopyDescriptorSet>& copies
	);

	MICRO_API void DeallocateDescriptors(
		const VkDevice& device, 
		const VkDescriptorPool& descriptor_pool, 
		std::vector<VkDescriptorSet>& descriptor_list 
	);

	MICRO_API VkResult CreateShader(
		const VkDevice& device,
		const VkShaderModuleCreateInfo& specification,
		VkShaderModule& shader
	);

	MICRO_API void DestroyShader( const VkDevice& device, VkShaderModule& shader );

	MICRO_API VkResult CreatePipeline(
		const VkDevice& device,
		const VkPipelineCache& cache,
		const VkGraphicsPipelineCreateInfo& specification,
		VkPipeline& pipeline
	);

	MICRO_API VkResult CreatePipeline(
		const VkDevice& device,
		const VkPipelineCache& cache,
		const VkComputePipelineCreateInfo& specification,
		VkPipeline& pipeline
	);

	MICRO_API void DestroyPipeline( const VkDevice& device, VkPipeline& pipeline );

	MICRO_API VkResult CreateQueryPool(
		const VkDevice& device,
		const VkQueryPoolCreateInfo& specification,
		VkQueryPool& query_pool
	);

	MICRO_API void DestroyQueryPool( const VkDevice& device, VkQueryPool& query_pool );

	MICRO_API VkResult WaitForFence(
		const VkDevice& device,
		const VkFence& fence,
		const uint64_t wait_time
	);

	MICRO_API VkResult ResetFence( 
		const VkDevice& device, 
		const VkFence& fence 
	);

	MICRO_API void CmdImageBarrier(
		const VkCommandBuffer& commands,
		const PipelineBarrier& specification,
		const VkImageMemoryBarrier& barrier
	);

	MICRO_API void CmdImageBarrier(
		const VkCommandBuffer& commands,
		const PipelineBarrier& specification,
		const std::vector<VkImageMemoryBarrier>& barriers
	);

	MICRO_API void CmdBufferBarrier(
		const VkCommandBuffer& commands,
		const PipelineBarrier& specification,
		const VkBufferMemoryBarrier& barrier
	);

	MICRO_API void CmdBufferBarrier(
		const VkCommandBuffer& commands,
		const PipelineBarrier& specification,
		const std::vector<VkBufferMemoryBarrier>& barriers
	);

	MICRO_API void CmdSetViewport(
		const VkCommandBuffer& commands,
		const VkViewport& viewport
	);

	MICRO_API void CmdSetViewport(
		const VkCommandBuffer& commands,
		const uint32_t start_id,
		const std::vector<VkViewport>& viewports
	);

	MICRO_API void CmdSetScissor(
		const VkCommandBuffer& commands,
		const VkScissor& scissor
	);

	MICRO_API void CmdSetScissor(
		const VkCommandBuffer& commands,
		const uint32_t start_id,
		const std::vector<VkScissor>& scissors
	);

	MICRO_API void CmdBindDescriptorSets(
		const VkCommandBuffer& commands,
		const VkPipelineBindPoint bind_point,
		const VkPipelineLayout& layout,
		const uint32_t start_id,
		const std::vector<VkDescriptorSet>& descriptor_sets
	);

	MICRO_API void CmdBindDescriptorSets(
		const VkCommandBuffer& commands,
		const VkPipelineBindPoint bind_point,
		const VkPipelineLayout& layout,
		const uint32_t start_id,
		const std::vector<VkDescriptorSet>& descriptor_sets,
		const std::vector<uint32_t> dymanic_offsets
	);

	MICRO_API void CmdBindVertexBuffers(
		const VkCommandBuffer& commands,
		const uint32_t start_id,
		const std::vector<BufferBindSpecification>& buffer_binds
	);

	MICRO_API VkResult AcquireNextImage(
		const VkDevice device,
		const VkSwapchainKHR swapchain,
		const uint64_t timeout,
		const VkSemaphore semaphore,
		uint32_t& image_index
	);

	MICRO_API VkResult QueueSubmit( 
		const VkQueue& queue,
		const VkFence& fence,
		const VkSubmitInfo& submit
	);

	MICRO_API VkResult QueueSubmit(
		const VkQueue& queue,
		const VkFence& fence,
		const std::vector<VkSubmitInfo>& submits
	);

	MICRO_API VkResult QueueSubmit(
		const VkQueue& queue,
		const VkFence& fence,
		const std::initializer_list<VkSubmitInfo> submits
	);

	MICRO_API VkResult QueuePresent( 
		const VkQueue& queue, 
		const VkPresentInfoKHR& present_info
	);

	MICRO_API void EnumerateInstanceExtension(
		std::vector<VkExtensionProperties>& extension_list 
	);

	MICRO_API void EnumeratePhysicalDevices(
		const VkInstance& instance, 
		std::vector<VkPhysicalDevice>& physical_list
	);

	MICRO_API void EnumeratePhysicalExtension(
		const VkPhysicalDevice& physical, 
		std::vector<VkExtensionProperties>& extensions 
	);

	MICRO_API void EnumeratePhysicalQueueFamilies(
		const VkPhysicalDevice& physical, 
		std::vector<VkQueueFamilyProperties>& queue_list 
	);

	/**
	 * EnumerateSwapchainFormats method
	 * @note : Wrapper for Vulkan surface format enumeration.
	 * @param physical : Reference to current Vulkan physical device instance.
	 * @param surface : Reference to current Vulkan surface instance.
	 * @param format_list : Reference to surface format vector.
	 **/
	MICRO_API void EnumerateSwapchainFormats(
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
	MICRO_API void EnumerateSwapchainPresentModes(
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
	MICRO_API void EnumerateSwapchainImages(
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
	MICRO_API void GetPhysicalSpecification(
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
	MICRO_API VkResult GetPipelineCacheData(
		const VkDevice& device,
		const VkPipelineCache& pipeline_cache,
		std::vector<uint8_t>& cache_data
	);

	MICRO_API micro_string ToString(
		const VkDebugUtilsMessageSeverityFlagBitsEXT debug_severity
	);

	MICRO_API micro_string ToString(
		const VkDebugUtilsMessageTypeFlagsEXT debug_type
	);

	MICRO_API micro_string ToString( const VkObjectType object_type );

	template<typename VkObject>
		requires IsVkObject<VkObject>
	bool IsValid( const VkObject& object ) {
		return object != VK_NULL_HANDLE;
	};

	template<typename VkFunction>
		requires IsVkObject<VkFunction>
	VkFunction GetInstanceProcAddr( 
		const VkInstance& instance, 
		micro_string procedure 
	) {
		return micro_cast( vkGetInstanceProcAddr( instance, procedure ), VkFunction );
	};

};
