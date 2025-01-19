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
MicroVulkanRenderPasses::MicroVulkanRenderPasses( )
    : m_passes{ }
{ }

bool MicroVulkanRenderPasses::Create( 
	const MicroVulkanDevice& device,
	const MicroVulkanSpecification& specification
) {
    auto pass_count = (uint32_t)specification.RenderPasses.size( );
    auto state      = true;

    m_passes.resize( pass_count );
	
    while ( state && pass_count-- > 0 )
        state = CreateRenderPass( device, specification.RenderPasses[ pass_count ], m_passes[ pass_count ] );

    return state;
}

void MicroVulkanRenderPasses::Destroy( const MicroVulkanDevice& device ) {
    for ( auto& render_pass : m_passes )
        vk::DestroyRenderPass( device, render_pass );
}

VkRenderPassCreateInfo MicroVulkanRenderPasses::CreateRenderPassSpec(
    const MicroVulkanRenderPass& render_pass_spec
) {
    auto specification = VkRenderPassCreateInfo{ };

    specification.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    specification.pNext           = VK_NULL_HANDLE;
    specification.flags           = VK_UNUSED_FLAG;
    specification.attachmentCount = (uint32_t)render_pass_spec.Attachements.size( );
    specification.pAttachments    = render_pass_spec.Attachements.data( );
    specification.subpassCount    = (uint32_t)render_pass_spec.Subpass.size( );
    specification.pSubpasses      = render_pass_spec.Subpass.data( );
    specification.dependencyCount = (uint32_t)render_pass_spec.Dependency.size( );
    specification.pDependencies   = render_pass_spec.Dependency.data( );

    return specification;
}

bool MicroVulkanRenderPasses::CreateRenderPass(
    const MicroVulkanDevice& device,
    const MicroVulkanRenderPass& render_pass_spec,
    VkRenderPass& render_pass
) {
    auto specification = CreateRenderPassSpec( render_pass_spec );

    return vk::CreateRenderPass( device, specification, render_pass ) == VK_SUCCESS;
}

const VkRenderPass MicroVulkanRenderPasses::Get( const uint32_t pass_id ) const {
    auto render_pass = (VkRenderPass)VK_NULL_HANDLE;

    if ( pass_id < (uint32_t)m_passes.size( ) )
        render_pass = m_passes[ pass_id ];

    return render_pass;
}
