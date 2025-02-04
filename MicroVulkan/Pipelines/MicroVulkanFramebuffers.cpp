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
MicroVulkanFramebuffers::MicroVulkanFramebuffers( ) 
    : m_dimensions_policy{ },
    m_framebuffers{ }
{ }

bool MicroVulkanFramebuffers::Create(
    const MicroVulkanWindow& window,
	const MicroVulkanDevice& device,
    const MicroVulkanQueues& queues,
    const MicroVulkanSwapchain& swapchain,
    const MicroVulkanRenderPasses& passes,
    const MicroVulkanSpecification& specification
) { 
    auto dimensions_spec = CreateDimensionsSpec( window );

    GetRenderFramebuffersSpec( specification );

    return CreateFramebuffers( device, queues, swapchain, passes, specification, dimensions_spec );
}

void MicroVulkanFramebuffers::Recreate(
    const MicroVulkanDevice& device,
    const MicroVulkanQueues& queues,
    const MicroVulkanSwapchain& swapchain,
    const MicroVulkanRenderPasses& passes,
    const micro_upoint& dimensions
) {
    auto dimensions_spec = CreateDimensionsSpec( dimensions );

    Destroy( device );

    RecreateFramebuffers( device, queues, swapchain, passes, dimensions );
}

void MicroVulkanFramebuffers::SetClearValue(
    const uint32_t render_pass_id,
    const uint32_t image_id,
    const VkClearValue& value
) {
    micro_assert( render_pass_id < (uint32_t)m_framebuffers.size( ), "You can't set Clear value for a render pass that doesn't have a framebuffer" );
    micro_assert( image_id < (uint32_t)m_framebuffers[ render_pass_id ].Clears.size( ), "You can't set Clear value for an image with an index out of framebuffer image list range" );

    auto& target = m_framebuffers[ render_pass_id ];

    target.Clears[ image_id ] = value;
}

void MicroVulkanFramebuffers::SetClearValue(
    const uint32_t render_pass_id,
    const std::vector<VkClearValue>& values
) {
    micro_assert( render_pass_id < (uint32_t)m_framebuffers.size( ), "You can't set Clear value for a render pass that doesn't have a framebuffer" );
    micro_assert( values.size( ) < m_framebuffers[ render_pass_id ].Clears.size( ), "You can't set Clear value for an image with an index out of framebuffer image list range" );

    auto image_count = (uint32_t)values.size( );
    auto& target     = m_framebuffers[ render_pass_id ];

    while ( image_count-- > 0 )
        target.Clears[ image_count ] = values[ image_count ];
}

MicroVulkanRenderPassInfo MicroVulkanFramebuffers::CreateRenderPassInfo(
    const uint32_t render_pass_id,
    const uint32_t frame_id,
    const VkRenderPass render_pass
) {
    auto render_pass_info = MicroVulkanRenderPassInfo{ };
    auto& framebuffer     = m_framebuffers[ render_pass_id ];
    auto& dimensions      = framebuffer.Dimensions;

    render_pass_info.BeginInfo = {
        VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        VK_NULL_HANDLE,
        render_pass,
        framebuffer.Targets[ frame_id ],
        { 0, 0, framebuffer.Dimensions.x, framebuffer.Dimensions.y },
        (uint32_t)framebuffer.Clears.size( ),
        framebuffer.Clears.data( )
    };
    render_pass_info.Viewport = {
        0.f, 0.f,
        (float)dimensions.x, (float)dimensions.y,
        0.f, 1.f
    };
    render_pass_info.Scissor = { 
        { 0, 0 },
        { dimensions.x, dimensions.y }
    };

    return render_pass_info;
}

void MicroVulkanFramebuffers::Destroy( const MicroVulkanDevice& device ) {
    auto frame_id = (uint32_t)0;

    for ( auto& framebuffer : m_framebuffers ) {
        for ( auto& target : framebuffer.Targets )
            DestroyFramebuffer( device, frame_id, target );

        frame_id += 1;
    }
}

micro_upoint MicroVulkanFramebuffers::CreateDimensionsSpec( 
    const MicroVulkanWindow& window
) {
    auto dimensions_spec = window.GetDimensions( );

    return CreateDimensionsSpec( dimensions_spec );
}

micro_upoint MicroVulkanFramebuffers::CreateDimensionsSpec( 
    const micro_upoint& dimensions 
) {
    m_dimensions_policy.Ratio;
    m_dimensions_policy.Scale;

    return dimensions;
}

void MicroVulkanFramebuffers::CreateTextureSampler(
    MicroVulkanTextureSpecification& texture_spec
) {
    auto& sampler_spec = texture_spec.Sampler;

    sampler_spec.sType                   = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    sampler_spec.pNext                   = VK_NULL_HANDLE;
    sampler_spec.flags                   = VK_UNUSED_FLAG;
    sampler_spec.magFilter               = VK_FILTER_NEAREST;
    sampler_spec.minFilter               = VK_FILTER_NEAREST;
    sampler_spec.mipmapMode              = VK_SAMPLER_MIPMAP_MODE_NEAREST;
    sampler_spec.addressModeU            = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
    sampler_spec.addressModeV            = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
    sampler_spec.addressModeW            = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
    sampler_spec.mipLodBias              = 0.f;
    sampler_spec.anisotropyEnable        = VK_FALSE;
    sampler_spec.maxAnisotropy           = 1.f;
    sampler_spec.compareEnable           = VK_FALSE;
    sampler_spec.compareOp               = VK_COMPARE_OP_NEVER;
    sampler_spec.minLod                  = 0.f;
    sampler_spec.maxLod                  =  .25f;
    sampler_spec.borderColor             = VK_BORDER_COLOR_INT_TRANSPARENT_BLACK;
    sampler_spec.unnormalizedCoordinates = VK_FALSE;
}

MicroVulkanTextureSpecification MicroVulkanFramebuffers::CreateTextureSpec(
    const micro_upoint& dimensions,
    const VkFormat format
) {
    auto texture_spec = MicroVulkanTextureSpecification{ };

    texture_spec.IsCubemap   = VK_FALSE;
    texture_spec.Type        = VK_IMAGE_TYPE_2D;
    texture_spec.Properties = {
        format,
        VK_IMAGE_LAYOUT_UNDEFINED,
        { dimensions.x, dimensions.y, 1 },
        1,
        1
    };
    texture_spec.Properties.Layout = GetTextureLayout( format );
    texture_spec.Samples     = VK_SAMPLE_COUNT_1_BIT;
    texture_spec.Usage       = GetTextureUsage( format );
    texture_spec.UseSampler  = VK_TRUE;

    CreateTextureSampler( texture_spec );

    return texture_spec;
}

bool MicroVulkanFramebuffers::CreateFramebufferSwapchain(
    const MicroVulkanDevice& device,
    const MicroVulkanQueues& queues,
    const std::vector<VkAttachmentDescription>& attachements_spec,
    const MicroVulkanSwapchainImage& swapchain_image,
    const micro_upoint& dimensions,
    MicroVulkanFrameTarget& target
) {
    auto texture_id = attachements_spec.size( );
    auto result     = true;

    target.Textures.resize( texture_id );

    while ( result && texture_id-- > 1 ) {
        auto texture_spec = CreateTextureSpec( dimensions, attachements_spec[ texture_id ].format );

        result = target.Textures[ texture_id ].Create( device, queues, texture_spec );
    }

    target.Textures[ 0 ].Create( swapchain_image );

    return result;
}

bool MicroVulkanFramebuffers::CreateFramebufferTextures(
    const MicroVulkanDevice& device,
    const MicroVulkanQueues& queues,
    const std::vector<VkAttachmentDescription>& attachements_spec,
    const micro_upoint& dimensions,
    MicroVulkanFrameTarget& target
) {
    auto texture_id = attachements_spec.size( );
    auto result     = true;

    target.Textures.resize( texture_id );

    while ( result && texture_id-- > 0 ) {
        auto texture_spec = CreateTextureSpec( dimensions, attachements_spec[ texture_id ].format );

        result = target.Textures[ texture_id ].Create( device, queues, texture_spec );
    }

    return result;
}

void MicroVulkanFramebuffers::RecreateFramebufferTextures(
    const MicroVulkanDevice& device,
    const MicroVulkanQueues& queues,
    const MicroVulkanSwapchainImage& image,
    const uint32_t render_pass_id,
    const micro_upoint& dimensions,
    MicroVulkanFrameTarget& target
) {
    auto texture_id = target.Textures.size( );

    while ( texture_id-- > ( render_pass_id > 0 ? 0 : 1 ) ) {
        auto texture_spec = CreateTextureSpec( dimensions, target.Textures[ texture_id ].Format );

        target.Textures[ texture_id ].Create( device, queues, texture_spec );
    }

    if ( render_pass_id == 0 )
        target.Textures[ 0 ].Create( image );
}

std::vector<VkImageView> MicroVulkanFramebuffers::CreateFramebufferAttachements(
    const MicroVulkanFrameTarget& target
) {
    auto attachement_id = target.Textures.size( );
    auto attachements   = std::vector<VkImageView>( attachement_id );

    while ( attachement_id-- > 0 )
        attachements[ attachement_id ] = target.Textures[ attachement_id ].GetView( );

    return attachements;
}

VkFramebufferCreateInfo MicroVulkanFramebuffers::CreateFramebufferSpec(
    const micro_upoint& dimensions,
    const VkRenderPass& render_pass,
    const std::vector<VkImageView>& attachements
) {
    auto specification = VkFramebufferCreateInfo{ };

    specification.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    specification.pNext           = VK_NULL_HANDLE;
    specification.flags           = VK_UNUSED_FLAG;
    specification.renderPass      = render_pass;
    specification.attachmentCount = (uint32_t)attachements.size( );
    specification.pAttachments    = attachements.data( );
    specification.width           = dimensions.x;
    specification.height          = dimensions.y;
    specification.layers          = 1;

    return specification;
}

bool MicroVulkanFramebuffers::CreateFramebuffer(
    const MicroVulkanDevice& device,
    const VkRenderPass& render_pass,
    const micro_upoint& dimensions,
    MicroVulkanFrameTarget& target
) {
    auto attachements     = CreateFramebufferAttachements( target );
    auto framebuffer_spec = CreateFramebufferSpec( dimensions, render_pass, attachements );

    return vk::CreateFramebuffer( device, framebuffer_spec, target.Framebuffer ) == VK_SUCCESS;
}

bool MicroVulkanFramebuffers::CreateFramebuffers(
    const MicroVulkanDevice& device,
    const MicroVulkanQueues& queues,
    const MicroVulkanSwapchain& swapchain,
    const MicroVulkanRenderPasses& passes,
    const MicroVulkanSpecification& specification,
    const micro_upoint& dimensions
) {
    auto render_pass_id = specification.RenderPasses.size( );
    auto frame_count    = swapchain.GetSpecification( ).ImageCount;
    auto result         = true;

    m_framebuffers.resize( render_pass_id );

    while ( result && render_pass_id-- > 0 ) {
        auto& render_pass_spec = specification.RenderPasses[ render_pass_id ];
        auto framebuffer_id    = frame_count;
        auto& framebuffer      = m_framebuffers[ render_pass_id ];

        framebuffer.Dimensions = dimensions;
        framebuffer.Clears     = render_pass_spec.ClearValues;

        framebuffer.Targets.resize( framebuffer_id );

        while ( result && framebuffer_id-- > 0 ) {
            auto render_pass = passes.Get( (uint32_t)render_pass_id );
            auto& target     = framebuffer.Targets[ framebuffer_id ];

            if ( render_pass_id == 0 ) {
                auto frame_texture = swapchain.GetImages( );

                result = CreateFramebufferSwapchain( device, queues, render_pass_spec.Attachements, frame_texture[ framebuffer_id ], dimensions, target ) &&
                         CreateFramebuffer( device, render_pass, dimensions, target );
            } else {
                result = CreateFramebufferTextures( device, queues, render_pass_spec.Attachements, dimensions, target ) &&
                         CreateFramebuffer( device, render_pass, dimensions, target );;
            }
        }
    }

    return result;
}

void MicroVulkanFramebuffers::RecreateFramebuffers(
    const MicroVulkanDevice& device,
    const MicroVulkanQueues& queues,
    const MicroVulkanSwapchain& swapchain,
    const MicroVulkanRenderPasses& passes,
    const micro_upoint& dimensions
) {
    auto render_pass_id = 0;
    auto frame_texture  = swapchain.GetImages( );

    for ( auto& framebuffer : m_framebuffers ) {
        auto render_pass = passes.Get( render_pass_id );
        auto target_id   = 0;

        framebuffer.Dimensions = dimensions;

        for ( auto& target : framebuffer.Targets ) {
            RecreateFramebufferTextures( device, queues, frame_texture[ target_id++ ], render_pass_id, dimensions, target );

            CreateFramebuffer( device, render_pass, dimensions, target );
        }

        render_pass_id += 1;
    }
}

void MicroVulkanFramebuffers::DestroyFramebuffer(
    const MicroVulkanDevice& device,
    const uint32_t frame_id,
    MicroVulkanFrameTarget& target
) {
    auto target_id = (uint32_t)0;

    for ( auto& texture : target.Textures ) {
        if ( frame_id != 0 && target_id != 0 )
            texture.Destroy( device );

        target_id += 1;
    }

    vk::DestroyFramebuffer( device, target.Framebuffer );
}

const MicroVulkanFramebuffer& MicroVulkanFramebuffers::Get(
    const uint32_t render_pass_id
) const {
    micro_assert( (size_t)render_pass_id < m_framebuffers.size( ), "Renderpass Index must be valid" );

    return m_framebuffers[ render_pass_id ];
}

micro_upoint MicroVulkanFramebuffers::GetDimensions(
    const uint32_t render_pass_id
) const {
    micro_assert( (size_t)render_pass_id < m_framebuffers.size( ), "Renderpass Index must be valid" );

    return m_framebuffers[ render_pass_id ].Dimensions;
}

std::vector<VkClearValue>& MicroVulkanFramebuffers::GetClearValues(
    const uint32_t render_pass_id 
) {
    micro_assert( (size_t)render_pass_id < m_framebuffers.size( ), "Renderpass Index must be valid" );

    return m_framebuffers[ render_pass_id ].Clears;
}

const std::vector<MicroVulkanFrameTarget>& MicroVulkanFramebuffers::GetTargets(
    const uint32_t render_pass_id
) const {
    micro_assert( (size_t)render_pass_id < m_framebuffers.size( ), "Renderpass Index must be valid" );

    return m_framebuffers[ render_pass_id ].Targets;
}

const MicroVulkanFrameTarget& MicroVulkanFramebuffers::GetTarget(
    const uint32_t render_pass_id,
    const uint32_t frame_id
) const {
    micro_assert( (size_t)render_pass_id < m_framebuffers.size( ), "Renderpass Index must be valid" );
    micro_assert( (size_t)frame_id < m_framebuffers[ render_pass_id ].Targets.size( ), "Frame Index must be valid" );

    return m_framebuffers[ render_pass_id ].Targets[ frame_id ];
}

VkImageLayout MicroVulkanFramebuffers::GetTextureLayout( 
    const VkFormat format 
) const {
    auto layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    if ( 
        format > VK_FORMAT_E5B9G9R9_UFLOAT_PACK32 && 
        format < VK_FORMAT_S8_UINT 
    )
        layout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
    else if ( format == VK_FORMAT_S8_UINT )
        layout = VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL;
    else if ( 
        format > VK_FORMAT_S8_UINT && 
        format < VK_FORMAT_BC1_RGB_UNORM_BLOCK 
    )
        layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    return layout;
}

VkImageUsageFlags MicroVulkanFramebuffers::GetTextureUsage(
    const VkFormat format
) const {
    auto usage = (VkImageUsageFlags)VK_IMAGE_USAGE_SAMPLED_BIT;

    if (
        format > VK_FORMAT_E5B9G9R9_UFLOAT_PACK32 &&
        format < VK_FORMAT_BC1_RGB_UNORM_BLOCK
    )
        usage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    else
        usage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    return usage;
}

void MicroVulkanFramebuffers::GetRenderFramebuffersSpec(
    const MicroVulkanSpecification& specification 
) {
    auto pass_count = (size_t)specification.RenderPasses.size( );

    m_dimensions_policy = specification.DimensionsPolicy;

    m_framebuffers.resize( pass_count );
}
