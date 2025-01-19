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
MicroMaterial::MicroMaterial( )
    : m_pipeline{ VK_NULL_HANDLE },
    m_layout{ VK_NULL_HANDLE },
    m_pool{ VK_NULL_HANDLE },
    m_descriptors{ }
{ }

bool MicroMaterial::Create( 
    const MicroVulkan& vulkan,
    const MicroMaterialSpecification& specification 
) {
    auto& pipelines = vulkan.GetPipelines( );
	auto& device = vulkan.GetDevice( );
    auto& passes = vulkan.GetPasses( );

    return  CreateLayout( device ) &&
            CreatePipeline( device, pipelines, passes, specification );
}

void MicroMaterial::Destroy( const MicroVulkan& vulkan ) {
	auto& device = vulkan.GetDevice( );

    vk::DeallocateDescriptors( device, m_pool, m_descriptors );
	vk::DestroyDescriptorPool( device, m_pool );
	vk::DestroyPipelineLayout( device, m_layout );
	vk::DestroyPipeline( device, m_pipeline );
}

bool MicroMaterial::CreateLayout(
    const MicroVulkanDevice& device
) {
    auto layout_spec = VkPipelineLayoutCreateInfo{ };

    layout_spec.sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    layout_spec.pNext                  = VK_NULL_HANDLE;
    layout_spec.flags                  = VK_UNUSED_FLAG;
    layout_spec.setLayoutCount         = 0;
    layout_spec.pSetLayouts            = nullptr;
    layout_spec.pushConstantRangeCount = 0;
    layout_spec.pPushConstantRanges    = nullptr;
    
    return vk::CreatePipelineLayout( device, layout_spec, m_layout ) == VK_SUCCESS;
}

bool MicroMaterial::CreateDescriporPool(
    const MicroVulkanDevice& device
) {
    auto pool_spec = VkDescriptorPoolCreateInfo{ };

    pool_spec.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    pool_spec.pNext         = VK_NULL_HANDLE;
    pool_spec.flags         = VK_UNUSED_FLAG;
    pool_spec.maxSets       = 0;
    pool_spec.poolSizeCount = 0;
    pool_spec.pPoolSizes    = nullptr;

    return vk::CreateDescriptorPool( device, pool_spec, m_pool ) == VK_SUCCESS;
}

bool MicroMaterial::CreateDescriptors(
    const MicroVulkanDevice& device
) {
    auto descriptor_spec = VkDescriptorSetAllocateInfo{ };

    descriptor_spec.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    descriptor_spec.pNext              = VK_NULL_HANDLE;
    descriptor_spec.descriptorPool     = m_pool;
    descriptor_spec.descriptorSetCount = 0;
    descriptor_spec.pSetLayouts        = nullptr;

    return vk::AllocateDescriptors( device, descriptor_spec, m_descriptors ) == VK_SUCCESS;
}

void MicroMaterial::CreateStageSpec(
    const MicroShaderSpecification& shader_spec,
    VkPipelineShaderStageCreateInfo& stage_spec
) {
    stage_spec.sType               = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    stage_spec.pNext               = VK_NULL_HANDLE;
    stage_spec.flags               = VK_UNUSED_FLAG;
    stage_spec.stage               = shader_spec.Stage;
    stage_spec.module              = VK_NULL_HANDLE;
    stage_spec.pName               = shader_spec.Name.c_str( );
    stage_spec.pSpecializationInfo = VK_NULL_HANDLE;
}

bool MicroMaterial::CreateShader( 
    const MicroVulkanDevice& device,
    const std::vector<uint32_t>& code,
    VkShaderModule& shader
) {
    auto shader_spec = VkShaderModuleCreateInfo{ };

    shader_spec.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shader_spec.pNext    = VK_NULL_HANDLE;
    shader_spec.flags    = VK_UNUSED_FLAG;
    shader_spec.codeSize = (uint32_t)code.size( );
    shader_spec.pCode    = code.data( );

    return vk::CreateShader( device, shader_spec, shader ) == VK_SUCCESS;
}

std::vector<VkPipelineShaderStageCreateInfo> MicroMaterial::CreatePipelineStagesSpec(
    const MicroVulkanDevice& device,
    const MicroMaterialSpecification& specification,
    bool& can_create
) {
    auto shader_count = specification.Shaders.size( );
    auto shader_stage_spec = std::vector<VkPipelineShaderStageCreateInfo>( shader_count );

    micro_assert( shader_count < 2, "You can't create a material without at least 2 shaders corresponding to Vertex & Fragment" );

    while ( can_create && shader_count-- > 0 ) {
        auto& shader_spec = specification.Shaders[ shader_count ];
        auto& stage_spec  = shader_stage_spec[ shader_count ];

        CreateStageSpec( shader_spec, stage_spec );

        can_create = CreateShader( device, shader_spec.Code, stage_spec.module ) == VK_SUCCESS;
    }

    return shader_stage_spec;
}

std::vector<VkDynamicState> MicroMaterial::CreateDynamicStates( ) {
    auto dynamic_states = std::vector<VkDynamicState>( 2 );

    dynamic_states[ 0 ] = VK_DYNAMIC_STATE_VIEWPORT;
    dynamic_states[ 1 ] = VK_DYNAMIC_STATE_SCISSOR;

    return dynamic_states;
}

VkPipelineDynamicStateCreateInfo MicroMaterial::CreateDynamicStateSpec(
    const std::vector<VkDynamicState>& dynamic_states
) {
    auto dynamic_state_spec = VkPipelineDynamicStateCreateInfo{ };

    dynamic_state_spec.sType             = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamic_state_spec.pNext             = VK_NULL_HANDLE;
    dynamic_state_spec.flags             = VK_UNUSED_FLAG;
    dynamic_state_spec.dynamicStateCount = (uint32_t)dynamic_states.size( );
    dynamic_state_spec.pDynamicStates    = dynamic_states.data( );

    return dynamic_state_spec;
}

VkGraphicsPipelineCreateInfo MicroMaterial::CreatePipelineSpec(
    const MicroVulkanRenderPasses& passes,
    const MicroMaterialSpecification& specification
) {
    auto pipeline_spec = VkGraphicsPipelineCreateInfo{ };

    pipeline_spec.sType              = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipeline_spec.pNext              = VK_NULL_HANDLE;
    pipeline_spec.flags              = VK_UNUSED_FLAG;
    pipeline_spec.layout             = m_layout;
    pipeline_spec.renderPass         = passes.Get( specification.RenderPass );
    pipeline_spec.subpass            = specification.Subpass;
    pipeline_spec.basePipelineHandle = VK_NULL_HANDLE;
    pipeline_spec.basePipelineIndex  = 0;

    return pipeline_spec;
}

bool MicroMaterial::CreatePipeline(
    const MicroVulkanDevice& device,
    const MicroVulkanPipelines& pipelines,
    const MicroVulkanRenderPasses& passes,
    const MicroMaterialSpecification& specification
) {
    auto can_create   = true;
    auto pipeline_spec = CreatePipelineSpec( passes, specification );
    auto dynamic_states = CreateDynamicStates( );

    auto stages_spec = CreatePipelineStagesSpec( device, specification, can_create );
    auto dynamic_state_spec = CreateDynamicStateSpec( dynamic_states );

    pipeline_spec.stageCount    = (uint32_t)stages_spec.size( );
    pipeline_spec.pStages       = stages_spec.data( );
    pipeline_spec.pVertexInputState = nullptr;
    pipeline_spec.pInputAssemblyState = nullptr;
    pipeline_spec.pTessellationState = nullptr;
    pipeline_spec.pViewportState = nullptr;
    pipeline_spec.pRasterizationState = nullptr;
    pipeline_spec.pMultisampleState = nullptr;
    pipeline_spec.pDepthStencilState = nullptr;
    pipeline_spec.pColorBlendState = nullptr;
    pipeline_spec.pDynamicState = &dynamic_state_spec;

    can_create = can_create && vk::CreatePipeline( device, pipelines.GetCache( ), pipeline_spec, m_pipeline ) == VK_SUCCESS;

    DestroyShaders( device, stages_spec );

    return can_create;
}

void MicroMaterial::DestroyShaders(
    const MicroVulkanDevice& device,
    std::vector<VkPipelineShaderStageCreateInfo>& stages
) {
    for ( auto& stage : stages )
        vk::DestroyShader( device, stage.module );
}
