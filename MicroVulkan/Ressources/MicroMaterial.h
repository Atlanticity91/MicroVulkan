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

#pragma once

#include "MicroTexture.h"

struct MicroShaderSpecification {

	VkShaderStageFlagBits Stage = VK_SHADER_STAGE_VERTEX_BIT;
	std::string Name = "";
	std::vector<uint32_t> Code{ };

};

struct MicroMaterialSpecification {

	uint32_t RenderPass = 0;
	uint32_t Subpass	= 0;
	std::vector<MicroShaderSpecification> Shaders{ };

};

class MicroMaterial {

private:
	VkPipeline m_pipeline;
	VkPipelineLayout m_layout;
	VkDescriptorPool m_pool;
	std::vector<VkDescriptorSet> m_descriptors;

public:
	MicroMaterial( );

	~MicroMaterial( ) = default;
	
	bool Create( 
		const MicroVulkan& vulkan,
		const MicroMaterialSpecification& specification
	);

	void Destroy( const MicroVulkan& vulkan );

private:
	bool CreateLayout(
		const MicroVulkanDevice& device
	);

	bool CreateDescriporPool(
		const MicroVulkanDevice& device
	);

	bool CreateDescriptors(
		const MicroVulkanDevice& device
	);

	void CreateStageSpec(
		const MicroShaderSpecification& shader_spec,
		VkPipelineShaderStageCreateInfo& stage_spec
	);

	bool CreateShader(
		const MicroVulkanDevice& device,
		const std::vector<uint32_t>& code,
		VkShaderModule& shader
	);

	std::vector<VkPipelineShaderStageCreateInfo> CreatePipelineStagesSpec(
		const MicroVulkanDevice& device,
		const MicroMaterialSpecification& specification,
		bool& can_create
	);

	virtual std::vector<VkDynamicState> CreateDynamicStates( );

	VkPipelineDynamicStateCreateInfo CreateDynamicStateSpec( 
		const std::vector<VkDynamicState>& dynamic_states
	);

	VkGraphicsPipelineCreateInfo CreatePipelineSpec(
		const MicroVulkanRenderPasses& passes,
		const MicroMaterialSpecification& specification
	);

	bool CreatePipeline(
		const MicroVulkanDevice& device,
		const MicroVulkanPipelines& pipelines,
		const MicroVulkanRenderPasses& passes,
		const MicroMaterialSpecification& specification
	);

	void DestroyShaders(
		const MicroVulkanDevice& device,
		std::vector<VkPipelineShaderStageCreateInfo>& stages
	);

};
