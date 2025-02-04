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
MicroVulkanTexture::MicroVulkanTexture( ) 
	: m_image{ VK_NULL_HANDLE },
	m_view{ VK_NULL_HANDLE },
	m_sampler{ VK_NULL_HANDLE },
	m_memory{ VK_NULL_HANDLE }
{ }

bool MicroVulkanTexture::Create( const MicroVulkanSwapchainImage& image ) {
    m_image = image.Image;
    m_view  = image.View;

    return vk::IsValid( m_image ) && vk::IsValid( m_view );
}

bool MicroVulkanTexture::Create( 
	const MicroVulkanDevice& device,
	const MicroVulkanQueues& queues,
    const MicroVulkanTextureSpecification& specification
) {
    return  CreateImage( device, queues, specification ) &&
            CreateStorage( device )                      &&
			CreateImageView( device, specification )     &&
			CreateImageSampler( device, specification );
}

void MicroVulkanTexture::Destroy( const MicroVulkanDevice& device ) {
    vk::DestroyImageSampler( device, m_sampler );
    vk::DestroyImageView( device, m_view );
    vk::DeallocateMemory( device, m_memory );
    vk::DestroyImage( device, m_image );
}

bool MicroVulkanTexture::CreateImage(
	const MicroVulkanDevice& device,
	const MicroVulkanQueues& queues,
    const MicroVulkanTextureSpecification& specification
) {
    auto image_spec = VkImageCreateInfo{ };

    image_spec.sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image_spec.pNext         = VK_NULL_HANDLE;
    image_spec.flags         = VK_UNUSED_FLAG;
    image_spec.imageType     = specification.Type;
    image_spec.format        = specification.Properties.Format;
    image_spec.extent        = specification.Properties.Extent;
    image_spec.mipLevels     = specification.Properties.MipLevels;
    image_spec.arrayLayers   = specification.Properties.ArrayLayers;
    image_spec.samples       = specification.Samples;
    image_spec.tiling        = VK_IMAGE_TILING_OPTIMAL;
    image_spec.usage         = specification.Usage;
    image_spec.initialLayout = VK_IMAGE_LAYOUT_GENERAL;
    
    GetQueueSharingPolicy( queues, image_spec );
    
    return vk::CreateImage( device, image_spec, m_image ) == VK_SUCCESS;
}

bool MicroVulkanTexture::CreateStorage( const MicroVulkanDevice& device ) {
    auto memory_spec = device.CreateImageAllocationSpec( m_image, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT );

    return  vk::AllocateMemory( device, memory_spec, m_memory ) == VK_SUCCESS &&
            vkBindImageMemory( device, m_image, m_memory, 0 ) == VK_SUCCESS;
}

bool MicroVulkanTexture::CreateImageView(
	const MicroVulkanDevice& device,
    const MicroVulkanTextureSpecification& specification
) {
    auto view_spec = VkImageViewCreateInfo{ };

    view_spec.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    view_spec.pNext                           = VK_NULL_HANDLE;
    view_spec.flags                           = VK_UNUSED_FLAG;
    view_spec.image                           = m_image;
    view_spec.viewType                        = GetImageViewType( specification );
    view_spec.format                          = specification.Properties.Format;
    view_spec.components                      = {
        VK_COMPONENT_SWIZZLE_R,
        VK_COMPONENT_SWIZZLE_G,
        VK_COMPONENT_SWIZZLE_B,
        VK_COMPONENT_SWIZZLE_A
    };
    view_spec.subresourceRange.aspectMask     = GetImageAspect( specification );
    view_spec.subresourceRange.baseMipLevel   = 0;
    view_spec.subresourceRange.levelCount     = specification.Properties.MipLevels;
    view_spec.subresourceRange.baseArrayLayer = 0;
    view_spec.subresourceRange.layerCount     = specification.Properties.ArrayLayers;

    return vk::CreateImageView( device, view_spec, m_view ) == VK_SUCCESS;
}

bool MicroVulkanTexture::CreateImageSampler(
	const MicroVulkanDevice& device,
    const MicroVulkanTextureSpecification& specification
) {
    auto sampler_spec = VkSamplerCreateInfo{ };
    auto result = VK_SUCCESS;

    if ( specification.UseSampler == VK_TRUE )
        result = vk::CreateImageSampler( device, sampler_spec, m_sampler );

    return result == VK_SUCCESS;
}

VkImage MicroVulkanTexture::GetImage( ) const {
    return m_image;
}

VkImageView MicroVulkanTexture::GetView( ) const {
    return m_view;
}

VkSampler MicroVulkanTexture::GetSampler( ) const {
    return m_sampler;
}

void MicroVulkanTexture::GetQueueSharingPolicy(
    const MicroVulkanQueues& queues,
    VkImageCreateInfo& image_spec
) const {
    image_spec.sharingMode           = VK_SHARING_MODE_EXCLUSIVE;
    image_spec.queueFamilyIndexCount = 0;
    image_spec.pQueueFamilyIndices   = VK_NULL_HANDLE;

    /*
    * TODO if graphics and present queue are not equal
    image_spec.sharingMode = VK_SHARING_MODE_CONCURRENT;
    image_spec.queueFamilyIndexCount = 2;
    image_spec.pQueueFamilyIndices = queueFamilyIndices;
    */
}

VkImageViewType MicroVulkanTexture::GetImageViewType(
    const MicroVulkanTextureSpecification& specification
) const {
    auto view_type = VK_IMAGE_VIEW_TYPE_2D;

    if ( specification.Type == VK_IMAGE_TYPE_1D )
        view_type = specification.Properties.ArrayLayers == 1 ? VK_IMAGE_VIEW_TYPE_1D : VK_IMAGE_VIEW_TYPE_1D_ARRAY;
    else if ( specification.Type == VK_IMAGE_TYPE_2D )
        view_type = specification.Properties.ArrayLayers == 1 ? VK_IMAGE_VIEW_TYPE_2D : VK_IMAGE_VIEW_TYPE_2D_ARRAY;
    else if ( specification.Type == VK_IMAGE_TYPE_3D ) {
        if ( specification.IsCubemap )
            view_type = specification.Properties.ArrayLayers == 1 ? VK_IMAGE_VIEW_TYPE_CUBE : VK_IMAGE_VIEW_TYPE_CUBE_ARRAY;
        else
            view_type = VK_IMAGE_VIEW_TYPE_3D;
    }

    return view_type;
}

VkImageAspectFlagBits MicroVulkanTexture::GetImageAspect( 
    const MicroVulkanTextureSpecification& specification 
) const {
    auto aspect = VK_IMAGE_ASPECT_COLOR_BIT;

    if (
        specification.Properties.Format == VK_FORMAT_D16_UNORM           ||
        specification.Properties.Format == VK_FORMAT_X8_D24_UNORM_PACK32 ||
        specification.Properties.Format == VK_FORMAT_D32_SFLOAT
    )
        aspect = VK_IMAGE_ASPECT_DEPTH_BIT;
    else if ( specification.Properties.Format == VK_FORMAT_S8_UINT )
        aspect = VK_IMAGE_ASPECT_STENCIL_BIT;
    else if ( 
        specification.Properties.Format > VK_FORMAT_S8_UINT &&
        specification.Properties.Format < VK_FORMAT_BC1_RGB_UNORM_BLOCK
    )
        aspect = (VkImageAspectFlagBits)( VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT );

    return aspect;
}

MicroVulkanTexture::operator VkImage ( ) const {
    return GetImage( );
}
