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
MicroTexture::MicroTexture( )
	: m_texture{ },
	m_specification{ }
{ }

bool MicroTexture::Create( MicroVulkan& vulkan ) {
    auto texture_spec = MicroVulkanTextureSpecification{ };
	
    return Create( vulkan, texture_spec );
}

bool MicroTexture::Create(
    MicroVulkan& vulkan,
    MicroVulkanTextureSpecification& specification
) {
    auto& device = vulkan.GetDevice( );
    auto& queues = vulkan.GetQueues( );

    m_specification = specification.Properties;

    return m_texture.Create( device, queues, specification );
}

void MicroTexture::Fill(
    MicroVulkan& vulkan,
    const uint32_t length,
    const uint8_t* pixels
) {
    auto& commands      = vulkan.GetCommands( );
    auto command_handle = commands.Acquire( vk::QUEUE_TYPE_GRAPHICS, VK_COMMAND_BUFFER_LEVEL_PRIMARY );
    auto barrier_spec   = vk::PipelineBarrier{ };
    auto image_spec     = VkImageMemoryBarrier{ }; 
    auto& queues        = vulkan.GetQueues( );
    
    image_spec = CreateTransitionSpec( queues, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL );

    barrier_spec.SrcStageMask = 0;
    barrier_spec.DstStageMask = 0;
    barrier_spec.DependencyFlags = 0;
    
    vk::CmdImageBarrier( command_handle.Buffer, barrier_spec, image_spec );

    image_spec = CreateTransitionSpec( queues, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL );

    barrier_spec.SrcStageMask = 0;
    barrier_spec.DstStageMask = 0;
    barrier_spec.DependencyFlags = 0;

    vk::CmdImageBarrier( command_handle.Buffer, barrier_spec, image_spec );
}

void MicroTexture::Destroy( const MicroVulkan& vulkan ) {
    auto& device = vulkan.GetDevice( );

    m_texture.Destroy( device );
}

VkImageMemoryBarrier MicroTexture::CreateTransitionSpec(
	const MicroVulkanQueues& queues,
    const VkImageLayout target_layout
) {
    auto transition_spec = VkImageMemoryBarrier{ };
    auto graphics_queue  = queues.GetQueueFamily( vk::QUEUE_TYPE_GRAPHICS );

    transition_spec.sType                           = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    transition_spec.pNext                           = VK_NULL_HANDLE;
    transition_spec.srcAccessMask                   = VK_ACCESS_NONE;
    transition_spec.dstAccessMask                   = VK_ACCESS_NONE;
    transition_spec.oldLayout                       = m_specification.Layout;
    transition_spec.newLayout                       = target_layout;
    transition_spec.srcQueueFamilyIndex             = graphics_queue;
    transition_spec.dstQueueFamilyIndex             = graphics_queue;
    transition_spec.image                           = m_texture;
    transition_spec.subresourceRange.aspectMask     = GetImageAspect( );
    transition_spec.subresourceRange.baseMipLevel   = 0;
    transition_spec.subresourceRange.levelCount     = m_specification.MipLevels;
    transition_spec.subresourceRange.baseArrayLayer = 0;
    transition_spec.subresourceRange.layerCount     = m_specification.ArrayLayers;

    m_specification.Layout = target_layout;

    return transition_spec;
}

const MicroVulkanTexture& MicroTexture::Get( ) const {
	return m_texture;
}

const MicroTextureProperties& MicroTexture::GetSpecification( ) const {
	return m_specification;
}

VkImageAspectFlagBits MicroTexture::GetImageAspect( ) const {
    auto aspect = VK_IMAGE_ASPECT_COLOR_BIT;

    if (
        m_specification.Format == VK_FORMAT_D16_UNORM ||
        m_specification.Format == VK_FORMAT_X8_D24_UNORM_PACK32 ||
        m_specification.Format == VK_FORMAT_D32_SFLOAT
    )
        aspect = VK_IMAGE_ASPECT_DEPTH_BIT;
    else if ( m_specification.Format == VK_FORMAT_S8_UINT )
        aspect = VK_IMAGE_ASPECT_STENCIL_BIT;
    else if (
        m_specification.Format > VK_FORMAT_S8_UINT &&
        m_specification.Format < VK_FORMAT_BC1_RGB_UNORM_BLOCK
    )
        aspect = (VkImageAspectFlagBits)( VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT );

    return aspect;
}

MicroTexture::operator const MicroVulkanTexture& ( ) const {
	return Get( );
}
