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
MicroTextureProperties::MicroTextureProperties( )
    : MicroTextureProperties{ VK_FORMAT_UNDEFINED, VK_IMAGE_LAYOUT_UNDEFINED, { }, 1, 1 }
{ }

MicroTextureProperties::MicroTextureProperties( const MicroTextureProperties& other )
    : MicroTextureProperties{ 
        other.Format, 
        other.Layout, 
        other.Extent, 
        other.MipLevels, 
        other.ArrayLayers 
    }
{ }

MicroTextureProperties::MicroTextureProperties( 
	const VkFormat format,
    const VkImageLayout layout
)
    : MicroTextureProperties{ format, layout, { }, 1, 1 }
{ }

MicroTextureProperties::MicroTextureProperties( 
	const VkFormat format,
	const VkImageLayout layout,
	const VkExtent3D& extend
)
    : MicroTextureProperties{ format, layout, extend, 1, 1 }
{ }

MicroTextureProperties::MicroTextureProperties( 
    const VkFormat format,
	const VkImageLayout layout,
	const VkExtent3D& extend,
	const uint32_t mip_levels,
	const uint32_t array_layers
)
    : Format{ format },
    Layout{ layout },
    Extent{ extend },
    MipLevels{ mip_levels },
	ArrayLayers{ array_layers }
{ }
