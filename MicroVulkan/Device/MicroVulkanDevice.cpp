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
MicroVulkanDevice::MicroVulkanDevice( )
	: m_specification{ },
    m_physical{ VK_NULL_HANDLE },
	m_device{ VK_NULL_HANDLE },
    m_memory{ }
{ }

bool MicroVulkanDevice::Create(
    const MicroVulkanInstance& instance,
    const MicroVulkanSpecification& specification
) {
	return  CreatePhysical( specification, instance ) &&
            CreateDevice( specification );
}

VkMemoryAllocateInfo MicroVulkanDevice::CreateImageAllocationSpec(
    const VkImage& image,
    const VkMemoryPropertyFlags properties
) const {
    auto allocation_spec = VkMemoryAllocateInfo{ };
    auto requirements    = VkMemoryRequirements{ };

    vkGetImageMemoryRequirements( m_device, image, micro_ptr( requirements ) );

    allocation_spec.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocation_spec.pNext           = VK_NULL_HANDLE;
    allocation_spec.allocationSize  = requirements.size;
    allocation_spec.memoryTypeIndex = GetPeekMemoryType( VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, requirements.memoryTypeBits );

    return allocation_spec;
}

VkMemoryAllocateInfo MicroVulkanDevice::CreateBufferAllocationSpec(
    const VkBuffer& buffer,
    const VkMemoryPropertyFlags properties
) const {
    auto allocation_spec = VkMemoryAllocateInfo{ };
    auto requirements    = VkMemoryRequirements{ };

    vkGetBufferMemoryRequirements( m_device, buffer, micro_ptr( requirements ) );

    allocation_spec.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocation_spec.pNext           = VK_NULL_HANDLE;
    allocation_spec.allocationSize  = requirements.size;
    allocation_spec.memoryTypeIndex = GetPeekMemoryType( VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, requirements.memoryTypeBits );

    return allocation_spec;
}

void MicroVulkanDevice::Wait( ) {
    vk::DeviceWait( m_device );
}

void MicroVulkanDevice::Destroy( ) {
	vk::DestroyDevice( m_device );
}

////////////////////////////////////////////////////////////////////////////////////////////
//		===	PRIVATE ===
////////////////////////////////////////////////////////////////////////////////////////////
void MicroVulkanDevice::PreSelectPhysical( std::vector<VkPhysicalDevice>& physical_list ) {
    auto properties = VkPhysicalDeviceProperties{ };
    auto list_first = physical_list.begin( );
    auto list_size  = physical_list.size( );

    while ( list_size-- > 0 ) {
        auto physical = physical_list[ list_size ];

        vkGetPhysicalDeviceProperties( physical, &properties );

        if ( properties.deviceType < 1 || properties.deviceType > 2 )
            physical_list.erase( list_first + list_size );
    }
}

uint32_t MicroVulkanDevice::CreatePhysicalScore( 
    const MicroVulkanSpecification& specification,
    const VkSurfaceKHR& surface,
    const VkPhysicalDevice& physical 
) {
    auto physical_spec = vk::DeviceSpecification{ };
    auto device_score  = (uint32_t)0;

    vk::GetPhysicalSpecification( physical, surface, physical_spec );

    if ( 
        GetPhysicalHasExtensions( specification, physical )  &&
        GetPhysicalHasFeatures( specification, physical_spec ) &&
        GetPhysicalHasDepth( specification, physical )
    ) {
        if ( physical_spec.Properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU )
            device_score += 1000;

        device_score += physical_spec.Properties.limits.maxImageDimension2D;
        device_score += GetPhysicalQueueScore( physical_spec );
    }

    return device_score;
}

void MicroVulkanDevice::SelectPhysical( 
    const MicroVulkanSpecification& specification, 
    const MicroVulkanInstance& instance,
    std::vector<VkPhysicalDevice>& physical_list 
) {
    auto device_score = (uint32_t)0;
    auto device_id    = UINT32_MAX;
    auto list_size    = (uint32_t)physical_list.size( );
    auto surface      = instance.GetSurface( );

    while ( list_size-- > 0 ) {
        auto physical = physical_list[ list_size ];
        auto score    = CreatePhysicalScore( specification, surface, physical );

        if ( device_score < score ) {
            device_score = score;
            device_id    = list_size;
        }
    }
    
    m_physical = physical_list[ device_id ];

    if ( vk::IsValid( m_physical ) ) {
        vk::GetPhysicalSpecification( m_physical, surface, m_specification );

        vkGetPhysicalDeviceMemoryProperties( m_physical, micro_ptr( m_memory ) );
    }
}

bool MicroVulkanDevice::CreatePhysical( 
    const MicroVulkanSpecification& specification, 
    const MicroVulkanInstance& instance 
) {
    auto physical_list = std::vector<VkPhysicalDevice>{ };

    vk::EnumeratePhysicalDevices( instance, physical_list );

    PreSelectPhysical( physical_list );
    SelectPhysical( specification, instance, physical_list );

    return vk::IsValid( m_physical );
}

std::vector<VkDeviceQueueCreateInfo> MicroVulkanDevice::CreatePhysicalQueues( 
    const std::vector<float>& priorities 
) {
    auto create_info = VkDeviceQueueCreateInfo{ };
    auto queue_list  = std::vector<VkDeviceQueueCreateInfo>{ };
    auto* queues     = micro_cast( m_specification.Queues, const uint32_t* );
    
    for ( auto queue_idx = 0; queue_idx < vk::QUEUE_TYPE_COUNT; queue_idx++ ) {
        auto queue_id = 2 * queue_idx;

        create_info.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        create_info.pNext            = VK_NULL_HANDLE;
        create_info.flags            = VK_UNUSED_FLAG;
        create_info.queueFamilyIndex = queues[ queue_id ];
        create_info.queueCount       = queues[ queue_id + 1 ];
        create_info.pQueuePriorities = priorities.data( );

        queue_list.emplace_back( create_info );
    }

    return queue_list;
}

bool MicroVulkanDevice::CreateDevice( const MicroVulkanSpecification& specification ) {
    auto queue_priorities = std::vector<float>( m_specification.Queues.Count, 1.f );
    auto create_info      = VkDeviceCreateInfo{ };
    auto queues           = CreatePhysicalQueues( queue_priorities );

    create_info.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    create_info.pNext                   = VK_NULL_HANDLE;
    create_info.flags                   = VK_UNUSED_FLAG;
    create_info.queueCreateInfoCount    = (uint32_t)queues.size( );
    create_info.pQueueCreateInfos       = queues.data( );
    create_info.enabledLayerCount       = (uint32_t)specification.Validations.size( );
    create_info.ppEnabledLayerNames     = specification.Validations.data( );
    create_info.enabledExtensionCount   = (uint32_t)specification.DeviceExtensions.size( );
    create_info.ppEnabledExtensionNames = specification.DeviceExtensions.data( );
    create_info.pEnabledFeatures        = VK_NULL_HANDLE;

	return vk::CreateDevice( m_physical, create_info, m_device ) == VK_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////////////////
//		===	PUBLIC GET ===
////////////////////////////////////////////////////////////////////////////////////////////
const vk::DeviceSpecification& MicroVulkanDevice::GetSpecification( ) const {
    return m_specification;
}

VkPhysicalDevice MicroVulkanDevice::GetPhysical( ) const {
	return m_physical;
}

VkDevice MicroVulkanDevice::GetDevice( ) const {
	return m_device;
}

uint32_t MicroVulkanDevice::GetPeekMemoryType(
    VkMemoryPropertyFlags properties,
    uint32_t requirement_bits
) const {
    auto memory_type_id = (uint32_t)0;

    while ( memory_type_id < m_memory.memoryTypeCount ) {
        if ( 
            ( requirement_bits & 1 ) && 
            ( m_memory.memoryTypes[ memory_type_id ].propertyFlags & properties ) == properties 
        )
            break;

        requirement_bits >>= 1;

        memory_type_id += 1;
    }

    micro_assert( memory_type_id < m_memory.memoryTypeCount, "Unable to find suitable memory type index for required properties and requirements bits combo" );

    return memory_type_id;
}

////////////////////////////////////////////////////////////////////////////////////////////
//		===	PRIVATE GET ===
////////////////////////////////////////////////////////////////////////////////////////////
bool MicroVulkanDevice::GetPhysicalHasExtensions(
    const MicroVulkanSpecification& specification,
    const VkPhysicalDevice& physical
) {
    auto device_extensions = std::vector<VkExtensionProperties>{ };
    auto extension_count   = (uint32_t)0;

    vk::EnumeratePhysicalExtension( physical, device_extensions );

    for ( auto& extension : specification.DeviceExtensions ) {
        for ( auto& current : device_extensions ) {
            if ( strcmp( current.extensionName, extension ) != 0 )
                continue;

            extension_count += 1;

            break;
        }
    }

    return extension_count == (uint32_t)specification.DeviceExtensions.size( );
}

bool MicroVulkanDevice::GetPhysicalHasFeatures(
    const MicroVulkanSpecification& specification,
    const vk::DeviceSpecification& physical_spec
) {
    return physical_spec.Features.geometryShader == VK_TRUE;
}

bool MicroVulkanDevice::GetPhysicalHasDepth(
    const MicroVulkanSpecification& specification,
    const VkPhysicalDevice& physical
) {
    auto properties = VkFormatProperties{ };
    auto has_depth  = false;

    for ( auto& depth_format : specification.Depths ) {
        vkGetPhysicalDeviceFormatProperties( physical, depth_format, micro_ptr( properties ) );

        if ( has_depth = ( properties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT ) )
            break;
    }

    return has_depth;
}

uint32_t MicroVulkanDevice::GetPhysicalQueueScore( 
    const vk::DeviceSpecification& physical_spec 
) {
    auto& queues = physical_spec.Queues;
    auto score   = (uint32_t)0;

    if (
        queues.Graphics  != queues.Transfert &&
        queues.Graphics  != queues.Compute   &&
        queues.Transfert != queues.Compute 
    )
        score = 4000;
    else if ( queues.Graphics == queues.Transfert && queues.Graphics != queues.Compute )
        score = 2000;
    else
        score = 1000;

    return score;
}

////////////////////////////////////////////////////////////////////////////////////////////
//		===	OPERATOR ===
////////////////////////////////////////////////////////////////////////////////////////////
MicroVulkanDevice::operator VkDevice ( ) const {
	return GetDevice( );
}
