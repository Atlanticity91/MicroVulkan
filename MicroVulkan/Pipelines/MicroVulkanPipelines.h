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

#include "MicroVulkanPipelineCacheHeader.h"

micro_class MicroVulkanPipelines final {

	constexpr static uint32_t CACHE_MAGIC = 0x434B564D;

private:
	std::string m_path;
	VkPipelineCache m_cache;
	mutable std::map<VkDescriptorType, uint32_t> m_limits;

public:
	MicroVulkanPipelines( );

	~MicroVulkanPipelines( ) = default;
	
	bool Create(
		const MicroVulkanDevice& device,
		const MicroVulkanSpecification& specification
	);

	void Destroy( const MicroVulkanDevice& device );

private:
	uint32_t CaculateCRC( const std::vector<uint8_t>& cache_data );

	std::vector<uint8_t> LoadCache( 
		const MicroVulkanDevice& device,
		const MicroVulkanSpecification& specification
	);

	VkPipelineCacheCreateInfo CreateCacheSpec(
		const MicroVulkanSpecification& specification,
		const std::vector<uint8_t>& cache_data
	);

	bool CreateCache( 
		const MicroVulkanDevice& device,
		const MicroVulkanSpecification& specification
	);

	void CreateLimtis( const MicroVulkanDevice& device );

	MicroVulkanPipelineCacheHeader CreateCacheHeader(
		const MicroVulkanDevice& device,
		const std::vector<uint8_t>& cache_data
	);

	void SaveCache( const MicroVulkanDevice& device );

public:
	const VkPipelineCache& GetCache( ) const;

	const std::map<VkDescriptorType, uint32_t>& GetLimits( ) const;

	uint32_t GetLimit( const VkDescriptorType type ) const;

};
