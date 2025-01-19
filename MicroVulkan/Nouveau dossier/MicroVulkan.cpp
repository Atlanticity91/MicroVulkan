#include <__micro_pch.h>

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

bool MicroVulkan::Create( const MicroWindow& window ) {
	auto specification = MicroVulkanSpecification{ };

	CreateVulkanSpec( window, specification );

	return Create( window, specification );
}

bool MicroVulkan::Create(
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

void MicroVulkan::Wait( ) {
	m_device.Wait( );
}

bool MicroVulkan::Acquire( 
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

MicroVulkanRenderPassInfo MicroVulkan::CreateRenderPassInfo(
	const MicroVulkanRenderContext& render_context,
	const uint32_t pass_id
) {
	auto render_pass = m_passes.Get( pass_id );
	auto pass_info   = m_framebuffers.CreateRenderPassInfo( pass_id, render_context.FrameID, render_pass );

	return std::move( pass_info );
}

void MicroVulkan::Present( 
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

void MicroVulkan::CreateExtensionsSpec(
	const MicroWindow& window,
	MicroVulkanSpecification& specification
) {
	CreateInstanceExtensionsSpec( window, specification );
	CreateDeviceExtensionsSpec( specification );
}

void MicroVulkan::CreateDepthSpec( MicroVulkanSpecification& specification ) {
	specification.Depths.emplace_back( VK_FORMAT_D24_UNORM_S8_UINT );
}

void MicroVulkan::CreateSwapchainPassSpec( MicroVulkanRenderPass& render_pass ) {
	// --- Attachements ---
	render_pass.Attachements.resize( 1 );

	render_pass.Attachements[ 0 ] = {
		VK_UNUSED_FLAG,
		VK_FORMAT_R8G8B8A8_UNORM,
		VK_SAMPLE_COUNT_1_BIT,
		VK_ATTACHMENT_LOAD_OP_CLEAR,
		VK_ATTACHMENT_STORE_OP_STORE,
		VK_ATTACHMENT_LOAD_OP_DONT_CARE,
		VK_ATTACHMENT_STORE_OP_DONT_CARE,
		VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
	};

	// --- CLEAR VALUES ---
	render_pass.ClearValues.resize( 1 );
	render_pass.ClearValues[ 0 ] = VkClearValue{ 1.f, 1.f, 1.f, 1.f };

	// --- References ---
	render_pass.References.resize( 1 );

	render_pass.References[ 0 ] = { 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };

	// --- Subpass ---
	render_pass.Subpass.resize( 1 );

	render_pass.Subpass[ 0 ] = { 
		VK_UNUSED_FLAG,
		VK_PIPELINE_BIND_POINT_GRAPHICS,
		0, VK_NULL_HANDLE,
		1, render_pass.References.data( ), VK_NULL_HANDLE,
		0, VK_NULL_HANDLE, VK_NULL_HANDLE
	};

	// --- Dependency ---
	render_pass.Dependency.resize( 1 );

	render_pass.Dependency[ 0 ] = {
		VK_SUBPASS_EXTERNAL, 
		0,
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
		VK_ACCESS_NONE, 
		VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_COLOR_ATTACHMENT_READ_BIT,
		VK_UNUSED_FLAG
	};
}

void MicroVulkan::CreateGeometryPassSpec( MicroVulkanRenderPass& render_pass ) {
	// --- Attachements ---
	render_pass.Attachements.resize( 6 );

	render_pass.Attachements[ 0 ] = {
		VK_UNUSED_FLAG,
		VK_FORMAT_R8G8B8A8_UNORM,
		VK_SAMPLE_COUNT_1_BIT,
		VK_ATTACHMENT_LOAD_OP_CLEAR,
		VK_ATTACHMENT_STORE_OP_STORE,
		VK_ATTACHMENT_LOAD_OP_DONT_CARE,
		VK_ATTACHMENT_STORE_OP_DONT_CARE,
		VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
	};
	render_pass.Attachements[ 1 ] = {
		VK_UNUSED_FLAG,
		VK_FORMAT_R8G8B8A8_UNORM,
		VK_SAMPLE_COUNT_1_BIT,
		VK_ATTACHMENT_LOAD_OP_CLEAR,
		VK_ATTACHMENT_STORE_OP_STORE,
		VK_ATTACHMENT_LOAD_OP_DONT_CARE,
		VK_ATTACHMENT_STORE_OP_DONT_CARE,
		VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
	};
	render_pass.Attachements[ 2 ] = {
		VK_UNUSED_FLAG,
		VK_FORMAT_R8G8B8A8_UNORM,
		VK_SAMPLE_COUNT_1_BIT,
		VK_ATTACHMENT_LOAD_OP_CLEAR,
		VK_ATTACHMENT_STORE_OP_STORE,
		VK_ATTACHMENT_LOAD_OP_DONT_CARE,
		VK_ATTACHMENT_STORE_OP_DONT_CARE,
		VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
	};
	render_pass.Attachements[ 3 ] = {
		VK_UNUSED_FLAG,
		VK_FORMAT_R8G8B8A8_UNORM,
		VK_SAMPLE_COUNT_1_BIT,
		VK_ATTACHMENT_LOAD_OP_CLEAR,
		VK_ATTACHMENT_STORE_OP_STORE,
		VK_ATTACHMENT_LOAD_OP_DONT_CARE,
		VK_ATTACHMENT_STORE_OP_DONT_CARE,
		VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
	};
	render_pass.Attachements[ 4 ] = {
		VK_UNUSED_FLAG,
		VK_FORMAT_R8G8B8A8_UNORM,
		VK_SAMPLE_COUNT_1_BIT,
		VK_ATTACHMENT_LOAD_OP_CLEAR,
		VK_ATTACHMENT_STORE_OP_STORE,
		VK_ATTACHMENT_LOAD_OP_DONT_CARE,
		VK_ATTACHMENT_STORE_OP_DONT_CARE,
		VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
	};

	render_pass.Attachements[ 5 ] = {
		VK_UNUSED_FLAG,
		VK_FORMAT_D24_UNORM_S8_UINT,
		VK_SAMPLE_COUNT_1_BIT,
		VK_ATTACHMENT_LOAD_OP_CLEAR,
		VK_ATTACHMENT_STORE_OP_DONT_CARE,
		VK_ATTACHMENT_LOAD_OP_DONT_CARE,
		VK_ATTACHMENT_STORE_OP_DONT_CARE,
		VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
	};

	// --- References ---
	render_pass.References.resize( 1 );

	render_pass.References[ 0 ] = { 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL }; // COLOR
	render_pass.References[ 1 ] = { 1, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL }; // POSITION
	render_pass.References[ 2 ] = { 2, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL }; // EMISSIVE
	render_pass.References[ 3 ] = { 3, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL }; // NORMAL
	render_pass.References[ 4 ] = { 4, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL }; // SPEC
	render_pass.References[ 5 ] = { 5, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL }; // DEPTH - STENCIL

	// --- Subpass ---
	render_pass.Subpass.resize( 1 );

	render_pass.Subpass[ 0 ] = {
		VK_UNUSED_FLAG,
		VK_PIPELINE_BIND_POINT_GRAPHICS,
		0, VK_NULL_HANDLE,
		5, render_pass.References.data( ), &render_pass.References[ 5 ],
		0, VK_NULL_HANDLE, VK_NULL_HANDLE
	};

	// --- Dependency ---
	render_pass.Dependency.resize( 2 );

	render_pass.Dependency[ 0 ] = {
		VK_SUBPASS_EXTERNAL,
		0,
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
		VK_ACCESS_NONE,
		VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_COLOR_ATTACHMENT_READ_BIT,
		VK_UNUSED_FLAG
	};

	render_pass.Dependency[ 1 ] = {
		VK_SUBPASS_EXTERNAL,
		0,
		VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
		VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
		VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
		VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT,
		VK_UNUSED_FLAG
	};
}

void MicroVulkan::CreateUIPassSpec( MicroVulkanRenderPass& render_pass ) {
	// --- Attachements ---
	render_pass.Attachements.resize( 1 );

	render_pass.Attachements[ 0 ] = {
		VK_UNUSED_FLAG,
		VK_FORMAT_R8G8B8A8_UNORM,
		VK_SAMPLE_COUNT_1_BIT,
		VK_ATTACHMENT_LOAD_OP_CLEAR,
		VK_ATTACHMENT_STORE_OP_STORE,
		VK_ATTACHMENT_LOAD_OP_DONT_CARE,
		VK_ATTACHMENT_STORE_OP_DONT_CARE,
		VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
	};

	// --- References ---
	render_pass.References.resize( 1 );

	render_pass.References[ 0 ] = { 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };

	// --- Subpass ---
	render_pass.Subpass.resize( 1 );

	render_pass.Subpass[ 0 ] = {
		VK_UNUSED_FLAG,
		VK_PIPELINE_BIND_POINT_GRAPHICS,
		0, VK_NULL_HANDLE,
		1, render_pass.References.data( ), VK_NULL_HANDLE,
		0, VK_NULL_HANDLE, VK_NULL_HANDLE
	};

	// --- Dependency ---
	render_pass.Dependency.resize( 1 );

	render_pass.Dependency[ 0 ] = {
		VK_SUBPASS_EXTERNAL,
		0,
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
		VK_ACCESS_NONE,
		VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_COLOR_ATTACHMENT_READ_BIT,
		VK_UNUSED_FLAG
	};
}

void MicroVulkan::CreateRenderPassSpec( MicroVulkanSpecification& specification ) {
	auto& render_passes = specification.RenderPasses;
	
	render_passes.resize( 1 );

	CreateSwapchainPassSpec( render_passes[ 0 ] );
	//CreateGeometryPassSpec( render_passes[ 1 ] );
	//CreateUIPassSpec( render_passes[ 2 ] );
}

void MicroVulkan::CreateDimensionsSpec(
	MicroVulkanSpecification& specification 
) {
	auto& dimensions_spec = specification.DimensionsPolicy;

	dimensions_spec.Ratio = MVK_RATIO_POLICY_16_9;
	dimensions_spec.Scale = MVK_SCALE_POLICY_NONE;
}

void MicroVulkan::CreateVulkanSpec( 
	const MicroWindow& window,
	MicroVulkanSpecification& specification 
) {
	CreateApplicationSpec( specification );
	CreateValidationSpec( specification );
	CreateExtensionsSpec( window, specification );
	CreateDepthSpec( specification );
	CreateRenderPassSpec( specification );
	CreateDimensionsSpec( specification );
}

bool MicroVulkan::CreateInstance(
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

bool MicroVulkan::CreatePipelines(
	const MicroWindow& window,
	const MicroVulkanSpecification& specification
) {
	return  m_commands.Create( m_device, m_queues, m_swapchain )							&&
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
	
	return vkAcquireNextImageKHR( m_device, m_swapchain, UINT64_MAX, render_context.Sync->Presentable, VK_NULL_HANDLE, &render_context.ImageID );
}

VkPresentInfoKHR MicroVulkan::CreatePresentSpec(
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

void MicroVulkan::DestroyRenderContext( MicroVulkanRenderContext& render_context ) {
	m_commands.Release( render_context.CommandBuffer );
	m_queues.Release( render_context.Queue );
}

void MicroVulkan::Recreate( const MicroWindow& window ) {
	auto dimensions = window.GetDimensions( );

	m_device.Wait( );

	m_swapchain.Recreate( m_instance, m_device, dimensions );
	m_framebuffers.Recreate( m_device, m_queues, m_swapchain, m_passes, dimensions );

	m_device.Wait( );
}

const MicroVulkanInstance& MicroVulkan::GetInstance( ) const {
	return m_instance;
}

const MicroVulkanDevice& MicroVulkan::GetDevice( ) const {
	return m_device;
}

MicroVulkanQueues& MicroVulkan::GetQueues( ) const {
	return m_queues;
}

const MicroVulkanSwapchain& MicroVulkan::GetSwapchain( ) const {
	return m_swapchain;
}

const MicroVulkanRenderPasses& MicroVulkan::GetPasses( ) const {
	return m_passes;
}

MicroVulkanStagings& MicroVulkan::GetStaging( ) const {
	return m_stagings;
}

MicroVulkanCommands& MicroVulkan::GetCommands( ) const {
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
