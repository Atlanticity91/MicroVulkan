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
//		===	INTERNAL ===
////////////////////////////////////////////////////////////////////////////////////////////
/*
 *  COPYRIGHT (C) 1986 Gary S. Brown.  You may use this program, or
 *  code or tables extracted from it, as desired without restriction.
 *
 *  First, the polynomial itself and its table of feedback terms.  The
 *  polynomial is
 *  X^32+X^26+X^23+X^22+X^16+X^12+X^11+X^10+X^8+X^7+X^5+X^4+X^2+X^1+X^0
 *
 *  Note that we take it "backwards" and put the highest-order term in
 *  the lowest-order bit.  The X^32 term is "implied"; the LSB is the
 *  X^31 term, etc.  The X^0 term (usually shown as "+1") results in
 *  the MSB being 1
 *
 *  Note that the usual hardware shift register implementation, which
 *  is what we're using (we're merely optimizing it by doing eight-bit
 *  chunks at a time) shifts bits into the lowest-order term.  In our
 *  implementation, that means shifting towards the right.  Why do we
 *  do it this way?  Because the calculated CRC must be transmitted in
 *  order from highest-order term to lowest-order term.  UARTs transmit
 *  characters in order from LSB to MSB.  By storing the CRC this way
 *  we hand it to the UART in the order low-byte to high-byte; the UART
 *  sends each low-bit to hight-bit; and the result is transmission bit
 *  by bit from highest- to lowest-order term without requiring any bit
 *  shuffling on our part.  Reception works similarly
 *
 *  The feedback terms table consists of 256, 32-bit entries.  Notes
 *
 *      The table can be generated at runtime if desired; code to do so
 *      is shown later.  It might not be obvious, but the feedback
 *      terms simply represent the results of eight shift/xor opera
 *      tions for all combinations of data and CRC register values
 *
 *      The values must be right-shifted by eight bits by the "updcrc
 *      logic; the shift must be unsigned (bring in zeroes).  On some
 *      hardware you could probably optimize the shift in assembler by
 *      using byte-swap instructions
 *      polynomial $edb88320
 *
 *
 * CRC32 code derived from work by Gary S. Brown.
 *
 * Table & implementation from https://web.mit.edu/freebsd/head/sys/libkern/crc32.c
 */

const uint32_t crc32_tab[] = {

	0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f,
	0xe963a535, 0x9e6495a3,	0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
	0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2,
	0xf3b97148, 0x84be41de,	0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
	0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec,	0x14015c4f, 0x63066cd9,
	0xfa0f3d63, 0x8d080df5,	0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
	0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,	0x35b5a8fa, 0x42b2986c,
	0xdbbbc9d6, 0xacbcf940,	0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
	0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423,
	0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
	0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d,	0x76dc4190, 0x01db7106,
	0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
	0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d,
	0x91646c97, 0xe6635c01, 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
	0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950,
	0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
	0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7,
	0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
	0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa,
	0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
	0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81,
	0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
	0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84,
	0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
	0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb,
	0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
	0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e,
	0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
	0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55,
	0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
	0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28,
	0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
	0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f,
	0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
	0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242,
	0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
	0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69,
	0x616bffd3, 0x166ccf45, 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
	0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc,
	0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
	0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693,
	0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
	0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d

};

////////////////////////////////////////////////////////////////////////////////////////////
//		===	PUBLIC ===
////////////////////////////////////////////////////////////////////////////////////////////
MicroVulkanPipelines::MicroVulkanPipelines( ) 
	: m_path{ "" },
	m_cache{ VK_NULL_HANDLE },
	m_limits{ }
{ }

bool MicroVulkanPipelines::Create(
	const MicroVulkanDevice& device,
	const MicroVulkanSpecification& specification
) {
	auto result = CreateCache( device, specification );

	if ( result )
		CreateLimtis( device );

	return result;
}

void MicroVulkanPipelines::Destroy(
	const MicroVulkanDevice& device
) {
	SaveCache( device );

	vk::DestroyPipelineCache( device, m_cache );
}

////////////////////////////////////////////////////////////////////////////////////////////
//		===	PRIVATE ===
////////////////////////////////////////////////////////////////////////////////////////////
uint32_t MicroVulkanPipelines::CaculateCRC( const std::vector<uint8_t>& cache_data ) {
	// Implementation From :
	// Table & implementation from https://web.mit.edu/freebsd/head/sys/libkern/crc32.c

	auto crc_value = (uint32_t)~0U;
	auto crc_count = cache_data.size( );
	auto* pointer  = cache_data.data( );

	while ( crc_count-- > 0 )
		crc_value = crc32_tab[ ( crc_value ^ ( *(pointer++) ) ) & 0xFF ] ^ ( crc_value >> 8 );

	return crc_value ^ ~0U;
}

std::vector<uint8_t> MicroVulkanPipelines::LoadCache(
	const MicroVulkanDevice& device,
	const MicroVulkanSpecification& specification
) {
	auto cache_data = std::vector<uint8_t>{ };

	m_path = specification.PipelineCache;

	if ( !m_path.empty( ) && std::filesystem::is_regular_file( m_path ) ) {
		auto* path = m_path.c_str( );

#		ifdef _WIN32
		auto* file = micro_cast( NULL, FILE* );

		if ( fopen_s( micro_ptr( file ), path, "rb" ) == 0 ) {
#		else
		file = fopen( path, "rb" );

		if ( file != NULL ) {
#		endif
			auto header = MicroVulkanPipelineCacheHeader{ };

			fread( micro_ptr( header.Magic  ), sizeof( uint32_t ), 1, file );
			fread( micro_ptr( header.Vendor ), sizeof( uint32_t ), 1, file );
			fread( micro_ptr( header.Device ), sizeof( uint32_t ), 1, file );
			fread( micro_ptr( header.CRC	), sizeof( uint32_t ), 1, file );
			
			if ( header.Magic == CACHE_MAGIC ) {
				auto cache_size = (size_t)0;

				fread( micro_ptr( cache_size ), sizeof( size_t ), 1, file );

				if ( cache_size > 0 ) {
					cache_data.resize( cache_size );

					fread( cache_data.data( ), sizeof( uint8_t ), cache_size, file );

					auto crc = CaculateCRC( cache_data );

					if ( header.CRC != crc )
						cache_data.clear( );
				}
			}

			fclose( file );
		}
	}

	return cache_data;
}

VkPipelineCacheCreateInfo MicroVulkanPipelines::CreateCacheSpec(
	const MicroVulkanSpecification& specification,
	const std::vector<uint8_t>& cache_data
) {
	auto cache_spec = VkPipelineCacheCreateInfo{ };
	
	cache_spec.sType		   = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
	cache_spec.pNext		   = VK_NULL_HANDLE;
	cache_spec.flags		   = VK_UNUSED_FLAG;
	cache_spec.initialDataSize = (uint32_t)cache_data.size( );
	cache_spec.pInitialData	   = micro_cast( cache_data.data( ), const void* );

	return cache_spec;
}

bool MicroVulkanPipelines::CreateCache(
	const MicroVulkanDevice& device,
	const MicroVulkanSpecification& specification
) {
	auto cache_data = LoadCache( device, specification );
	auto cache_spec = CreateCacheSpec( specification, cache_data );

	return vk::CreatePipelineCache( device, cache_spec, m_cache ) == VK_SUCCESS;
}

void MicroVulkanPipelines::CreateLimtis( const MicroVulkanDevice& device ) {
	auto& device_limits = device.GetSpecification( ).Properties.limits;

	m_limits.emplace( VK_DESCRIPTOR_TYPE_SAMPLER			   , device_limits.maxDescriptorSetSamplers				 );	
	m_limits.emplace( VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, device_limits.maxDescriptorSetSampledImages		 );
	m_limits.emplace( VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE		   , device_limits.maxDescriptorSetSampledImages		 );
	m_limits.emplace( VK_DESCRIPTOR_TYPE_STORAGE_IMAGE		   , device_limits.maxDescriptorSetStorageImages		 );
	m_limits.emplace( VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER  , device_limits.maxDescriptorSetSampledImages		 );
	m_limits.emplace( VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER  , device_limits.maxDescriptorSetStorageImages		 );
	m_limits.emplace( VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER		   , device_limits.maxDescriptorSetUniformBuffers		 );
	m_limits.emplace( VK_DESCRIPTOR_TYPE_STORAGE_BUFFER		   , device_limits.maxDescriptorSetStorageBuffers		 );
	m_limits.emplace( VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, device_limits.maxDescriptorSetUniformBuffersDynamic );
	m_limits.emplace( VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, device_limits.maxDescriptorSetStorageBuffersDynamic );
	m_limits.emplace( VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT	   , device_limits.maxDescriptorSetInputAttachments		 );
}

MicroVulkanPipelineCacheHeader MicroVulkanPipelines::CreateCacheHeader(
	const MicroVulkanDevice& device,
	const std::vector<uint8_t>& cache_data
) {
	auto& device_properties = device.GetSpecification( ).Properties;
	auto cache_header		= MicroVulkanPipelineCacheHeader{ };

	cache_header.Magic  = CACHE_MAGIC;
	cache_header.Vendor = device_properties.vendorID;
	cache_header.Device = device_properties.deviceID;
	cache_header.CRC	= CaculateCRC( cache_data );

	return cache_header;
}

void MicroVulkanPipelines::SaveCache( const MicroVulkanDevice& device ) {
	if ( !m_path.empty( ) && vk::IsValid( m_cache ) ) {
		auto cache_data = std::vector<uint8_t>{ };

		if ( vk::GetPipelineCacheData( device, m_cache, cache_data ) == VK_SUCCESS ) {
			auto* path = m_path.c_str( );

#			ifdef _WIN32
			auto* file = micro_cast( NULL, FILE* );

			if ( fopen_s( micro_ptr( file ), path, "wb" ) == 0 ) {
#			else
			file = fopen( path, "wb" );

			if ( file != NULL ) {
#			endif
				auto cache_size = cache_data.size( );
				auto header		= CreateCacheHeader( device, cache_data );
				
				fwrite( micro_ptr( header.Magic  ), sizeof( uint32_t ), 1, file );
				fwrite( micro_ptr( header.Vendor ), sizeof( uint32_t ), 1, file );
				fwrite( micro_ptr( header.Device ), sizeof( uint32_t ), 1, file );
				fwrite( micro_ptr( header.CRC	 ), sizeof( uint32_t ), 1, file );

				fwrite( micro_ptr( cache_size ), sizeof( size_t ), 1, file );
				fwrite( cache_data.data( ), sizeof( uint8_t ), cache_size, file );

				fclose( file );
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
//		=== LPUBLIC GET ===
////////////////////////////////////////////////////////////////////////////////////////////
const VkPipelineCache& MicroVulkanPipelines::GetCache( ) const {
	return m_cache;
}

const std::map<VkDescriptorType, uint32_t>& MicroVulkanPipelines::GetLimits( ) const {
	return m_limits;
}

uint32_t MicroVulkanPipelines::GetLimit( const VkDescriptorType descriptor ) const {
	auto limit = (uint32_t)0;

	if ( m_limits.contains( descriptor ) )
		limit = m_limits[ descriptor ];

	return limit;
}
