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
MicroVulkanTextureSpecification::MicroVulkanTextureSpecification( )
    : MicroVulkanTextureSpecification{
        {
            VK_FORMAT_R8G8B8A8_SRGB,
            VK_IMAGE_LAYOUT_GENERAL,
            { 0, 0, 0 },
            1,
            1
        },
        VK_IMAGE_TYPE_2D,
        VK_FALSE,
        VK_SAMPLE_COUNT_1_BIT,
        MVT_USAGE_TEXTURE,
        { }
    }
{ }

MicroVulkanTextureSpecification::MicroVulkanTextureSpecification( 
    const MicroVulkanTextureSpecification& other
)
    : MicroVulkanTextureSpecification{
        other.Properties,
        other.Type,
        VK_FALSE,
        other.Samples,
        MVT_USAGE_TEXTURE,
        other.Sampler
    }
{
    Usage      = other.Usage;
    IsCubemap  = other.IsCubemap;
    UseSampler = other.UseSampler;
}

MicroVulkanTextureSpecification::MicroVulkanTextureSpecification( 
    const MicroTextureProperties& properties 
)
    : MicroVulkanTextureSpecification{
        properties,
        VK_IMAGE_TYPE_2D,
        VK_FALSE,
        VK_SAMPLE_COUNT_1_BIT,
        MVT_USAGE_TEXTURE,
        { }
    }
{ }

MicroVulkanTextureSpecification::MicroVulkanTextureSpecification(
    const MicroTextureProperties& properties,
    const VkImageType Type,
    const MicroVulkanTextureUsage usage
)
    : MicroVulkanTextureSpecification{
        properties,
        Type,
        VK_FALSE,
        VK_SAMPLE_COUNT_1_BIT,
        usage,
        { }
    }
{ }

MicroVulkanTextureSpecification::MicroVulkanTextureSpecification(
    const MicroTextureProperties& properties,
    const VkImageType Type,
    const VkBool32 is_cubemap,
    const VkSampleCountFlagBits samples,
    const MicroVulkanTextureUsage usage,
    const VkSamplerCreateInfo sampler
)    
    : Properties{ properties },
    Type{ Type },
    Samples{ samples },
    Usage{ (VkImageUsageFlags)usage },
    Sampler{ sampler },
    IsCubemap{ is_cubemap },
    UseSampler{ VK_TRUE }
{ }
