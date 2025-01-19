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
MicroVulkanSwapchain::MicroVulkanSwapchain( )
	: m_specification{ },
    m_swapchain{ VK_NULL_HANDLE },
    m_images{ }
{ }

bool MicroVulkanSwapchain::Create( 
	const MicroWindow& window,
    const MicroVulkanInstance& instance,
	const MicroVulkanDevice& device 
) {
	return  CreateSwapchain( window, instance, device ) &&
            CreateImages( device );
}

void MicroVulkanSwapchain::Recreate(
    const MicroVulkanInstance& instance,
	const MicroVulkanDevice& device,
	const micro_upoint& dimensions 
) {
	auto* old_swapchain = m_swapchain;
    auto specification  = CreateSwapchainSpec( instance, device, dimensions );

    specification.oldSwapchain = old_swapchain;

    DestroyImages( device );

	vk::CreateSwapchain( device, specification, m_swapchain );
    vk::DestroySwapchain( device, old_swapchain );

    CreateImages( device );
}

void MicroVulkanSwapchain::Destroy( const MicroVulkanDevice& device ) {
    DestroyImages( device );

    vk::DestroySwapchain( device, m_swapchain );
}

void MicroVulkanSwapchain::CreateSwapchainSurface(
    const MicroVulkanInstance& instance,
    const VkSurfaceCapabilitiesKHR& surface_spec,
    VkSwapchainCreateInfoKHR& specification
) {
    specification.surface = instance.GetSurface( );

    if ( surface_spec.minImageCount < surface_spec.maxImageCount && 3 <= surface_spec.maxImageCount )
        specification.minImageCount = 3;
    else
        specification.minImageCount = surface_spec.minImageCount;

    specification.preTransform  = surface_spec.currentTransform;
}

void MicroVulkanSwapchain::CreateSwapchainFormat(
    const MicroVulkanInstance& instance,
    const MicroVulkanDevice& device,
    VkSwapchainCreateInfoKHR& specification
) {
    auto surface_format = GetSurfaceFormat( instance, device );

    specification.imageFormat     = surface_format.format;
    specification.imageColorSpace = surface_format.colorSpace;
}

void MicroVulkanSwapchain::CreateSwapchainExtent(
    const VkSurfaceCapabilitiesKHR& surface_spec,
    const micro_upoint& dimensions,
    VkSwapchainCreateInfoKHR& specification
) {
    auto min_extent = surface_spec.minImageExtent;
    auto max_extent = surface_spec.maxImageExtent;

    specification.imageExtent = { 
        std::clamp( dimensions.x, min_extent.width , max_extent.width  ),
        std::clamp( dimensions.y, min_extent.height, max_extent.height )
    };
}

void MicroVulkanSwapchain::CreateSwapchainPresent(
    const MicroVulkanInstance& instance,
    const MicroVulkanDevice& device,
    VkSwapchainCreateInfoKHR& specification
) {
    auto present_modes = std::vector<VkPresentModeKHR>{ };
    auto physical      = device.GetPhysical( );
    auto surface       = instance.GetSurface( );

    vk::EnumerateSwapchainPresentModes( physical, surface, present_modes );

    specification.presentMode = VK_PRESENT_MODE_MAILBOX_KHR;

    for ( auto& present_mode : present_modes ) {
        if ( present_mode == VK_PRESENT_MODE_FIFO_KHR ) {
            specification.presentMode = present_mode;

            break;
        }
    }
}

VkSwapchainCreateInfoKHR MicroVulkanSwapchain::CreateSwapchainSpec(
    const MicroVulkanInstance& instance,
    const MicroVulkanDevice& device,
    const micro_upoint& dimensions
) {
    auto specification = VkSwapchainCreateInfoKHR{ };
    auto surface_spec  = GetSurfaceSpec( instance, device );

    specification.sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    specification.pNext            = VK_NULL_HANDLE;
    specification.flags            = VK_UNUSED_FLAG;
    specification.imageArrayLayers = 1;
    specification.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    specification.compositeAlpha   = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    specification.clipped          = VK_TRUE;
    specification.oldSwapchain     = VK_NULL_HANDLE;

    CreateSwapchainSurface( instance, surface_spec, specification );
    CreateSwapchainFormat( instance, device, specification );
    CreateSwapchainExtent( surface_spec, dimensions, specification );
    GetQueueSharingPolicy( device, specification );
    CreateSwapchainPresent( instance, device, specification );

    GetSwapchainSpecification( specification );
    
    return specification;
}

bool MicroVulkanSwapchain::CreateSwapchain(
    const MicroWindow& window,
    const MicroVulkanInstance& instance,
    const MicroVulkanDevice& device
) {
    auto dimensions    = window.GetDimensions( );
    auto specification = CreateSwapchainSpec( instance, device, dimensions );

    return vk::CreateSwapchain( device, specification, m_swapchain ) == VK_SUCCESS;
}

bool MicroVulkanSwapchain::CreateImages( const MicroVulkanDevice& device ) {
    auto image_list = std::vector<VkImage>{ };
    auto state      = true;

    vk::EnumerateSwapchainImages( device, m_swapchain, image_list );

    auto image_id = (uint32_t)image_list.size( );

    m_images.resize( image_id );

    while ( state && image_id-- > 0 ) {
        auto specification = VkImageViewCreateInfo{ };

        specification.sType      = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        specification.pNext      = VK_NULL_HANDLE;
        specification.flags      = VK_UNUSED_FLAG;
        specification.image      = image_list[ image_id ];
        specification.viewType   = VK_IMAGE_VIEW_TYPE_2D;
        specification.format     = m_specification.Format;
        specification.components = {
            VK_COMPONENT_SWIZZLE_IDENTITY,
            VK_COMPONENT_SWIZZLE_IDENTITY,
            VK_COMPONENT_SWIZZLE_IDENTITY,
            VK_COMPONENT_SWIZZLE_IDENTITY
        };
        specification.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };

        m_images[ image_id ].Image = image_list[ image_id ];

        state = vk::CreateImageView( device, specification, m_images[ image_id ].View ) == VK_SUCCESS;
    }

    return state;
}

void MicroVulkanSwapchain::DestroyImages( const MicroVulkanDevice& device ) {
    for ( auto& image : m_images ) {
        if ( vk::IsValid( image.Image ) ) {
            image.Image = VK_NULL_HANDLE;

            vk::DestroyImageView( device, image.View );
        }
    }
}

const MicroVulkanSwapchainSpecification& MicroVulkanSwapchain::GetSpecification( ) const {
    return m_specification;
}

VkSwapchainKHR MicroVulkanSwapchain::Get( ) const {
	return m_swapchain;
}

const std::vector<MicroVulkanSwapchainImage> MicroVulkanSwapchain::GetImages( ) const {
    return std::move( m_images );
}

VkSurfaceCapabilitiesKHR MicroVulkanSwapchain::GetSurfaceSpec(
    const MicroVulkanInstance& instance,
    const MicroVulkanDevice& device
) const {
    auto surface_spec = VkSurfaceCapabilitiesKHR{ };
    auto physical     = device.GetPhysical( );
    auto surface      = instance.GetSurface( );

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR( physical, surface, &surface_spec );

    return surface_spec;
}

VkSurfaceFormatKHR MicroVulkanSwapchain::GetSurfaceFormat(
    const MicroVulkanInstance& instance,
    const MicroVulkanDevice& device
) const {
    auto format_list = std::vector<VkSurfaceFormatKHR>{ };
    auto format_id   = (uint32_t)0;
    auto physical    = device.GetPhysical( );
    auto surface     = instance.GetSurface( );

    vk::EnumerateSwapchainFormats( physical, surface, format_list );

    for ( auto& format : format_list ) {
        if ( 
            format.format     == VK_FORMAT_R8G8B8A8_UNORM &&
            format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR 
        )
            break;

        format_id += 1;
    }

    return format_list[ format_id ];
}

void MicroVulkanSwapchain::GetQueueSharingPolicy(
    const MicroVulkanDevice& device,
    VkSwapchainCreateInfoKHR& specification
) {
    specification.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE;
    specification.queueFamilyIndexCount = 0;
    specification.pQueueFamilyIndices   = VK_NULL_HANDLE;

    /*
    * TODO if graphics and present queue are not equal
    specification.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    specification.queueFamilyIndexCount = 2;
    specification.pQueueFamilyIndices = queueFamilyIndices;
    */
}

void MicroVulkanSwapchain::GetSwapchainSpecification(
    const VkSwapchainCreateInfoKHR& swapchain_info
) {
    m_specification.ImageCount  = swapchain_info.minImageCount;
    m_specification.Format      = swapchain_info.imageFormat;
    m_specification.ColorSpace  = swapchain_info.imageColorSpace;
    m_specification.PresentMode = swapchain_info.presentMode;
}

MicroVulkanSwapchain::operator VkSwapchainKHR ( ) const {
	return Get( );
}

MicroVulkanSwapchain::operator VkSwapchainKHR* ( ) const {
    return (VkSwapchainKHR*)&m_swapchain;
}
