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
MicroVulkanInstance::MicroVulkanInstance( )
	: m_instance{ VK_NULL_HANDLE },
    m_debug_messenger{ VK_NULL_HANDLE },
	m_surface{ VK_NULL_HANDLE }
{ }

bool MicroVulkanInstance::Create( 
    const MicroVulkanWindow& window,
    const MicroVulkanSpecification& specification
) {
    return  CreateInstance( specification ) &&
            CreateDebugMessenger( )         &&
            CreateSurface( window );
}

void MicroVulkanInstance::Destroy( ) {
    vk::DestroySurface( m_instance, m_surface );
    vk::DestroyDebugMessenger( m_instance, m_debug_messenger );
    vk::DestroyInstance( m_instance );
}

////////////////////////////////////////////////////////////////////////////////////////////
//		===	PRIVATE ===
////////////////////////////////////////////////////////////////////////////////////////////
bool MicroVulkanInstance::CreateInstance( const MicroVulkanSpecification& specification ) {
    auto create_info = VkInstanceCreateInfo{ };

    create_info.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pNext                   = VK_NULL_HANDLE;
    create_info.flags                   = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
    create_info.pApplicationInfo        = micro_ptr (specification.Application );
    create_info.enabledLayerCount       = (uint32_t)specification.Validations.size( );
    create_info.ppEnabledLayerNames     = specification.Validations.data( );
    create_info.enabledExtensionCount   = (uint32_t)specification.InstanceExtensions.size( );
    create_info.ppEnabledExtensionNames = specification.InstanceExtensions.data( );

    return vk::CreateInstance( create_info, m_instance ) == VK_SUCCESS;
}

bool MicroVulkanInstance::CreateDebugMessenger( ) {
#	ifdef _DEBUG 
    auto create_info = VkDebugUtilsMessengerCreateInfoEXT{ };

    create_info.sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    create_info.pNext           = VK_NULL_HANDLE;
    create_info.flags           = VK_UNUSED_FLAG;
    create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    create_info.messageType     = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    create_info.pfnUserCallback = DebugCallback;
    create_info.pUserData       = VK_NULL_HANDLE;

    return vk::CreateDebugMessenger( m_instance, create_info, m_debug_messenger ) == VK_SUCCESS;
#   else
    return true;
#   endif
}

bool MicroVulkanInstance::CreateSurface( const MicroVulkanWindow& window ) {
    return window.CreateVKSurface( m_instance, m_surface );
}

////////////////////////////////////////////////////////////////////////////////////////////
//		===	PRIVATE STATIC ===
////////////////////////////////////////////////////////////////////////////////////////////
VkBool32 MicroVulkanInstance::DebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT severity,
    VkDebugUtilsMessageTypeFlagsEXT type,
    const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
    void* user_data
) {
    micro_unused( user_data );

    auto queues   = std::string{ "Queues :\n" };
    auto queue_id = callback_data->queueLabelCount;

    while ( queue_id-- > 0 )
        queues += std::format( "\t%s\n", callback_data->pQueueLabels[ queue_id ].pLabelName );

    auto commands   = std::string{ "Commands :\n" };
    auto command_id = callback_data->cmdBufLabelCount;

    while ( command_id-- > 0 )
        commands += std::format( "\t%s\n", callback_data->pCmdBufLabels[ command_id ].pLabelName );

    auto objects   = std::string{ "Dump : \n" };
    auto object_id = callback_data->objectCount;

    while ( object_id-- > 0 ) {
        const auto& object = callback_data->pObjects[ object_id ];

        objects += std::format( 
            "\t[ %u ] %s : %s \n", 
            object.objectHandle, 
            vk::ToString( object.objectType ), 
            object.pObjectName 
        );
    }

    const auto formated_message = std::format( 
        "[ VK - %s : %s ]%s : %u\n%s\n%s\n%s\n%s\n",
        vk::ToString( type ),
        vk::ToString( severity ),
        callback_data->pMessageIdName, 
        callback_data->messageIdNumber, 
        callback_data->messageIdNumber,
        queues,
        commands,
        objects
    );
    const auto* debug_message = formated_message.c_str( );

    switch ( severity ) {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT :
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT    :
            micro::log_info( "%s", debug_message );
            break;

        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: 
            micro::log_warn( "%s", debug_message );
            break;

        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            micro::log_errr( "%s", debug_message );
            break;

        default : break;
    }

    return ( severity < VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT ) ? VK_FALSE : VK_TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////
//		===	PUBLIC GET ===
////////////////////////////////////////////////////////////////////////////////////////////
VkInstance MicroVulkanInstance::GetInstance( ) const {
    return m_instance;
}

VkSurfaceKHR MicroVulkanInstance::GetSurface( ) const {
    return m_surface;
}

////////////////////////////////////////////////////////////////////////////////////////////
//		===	OPERATOR ===
////////////////////////////////////////////////////////////////////////////////////////////
MicroVulkanInstance::operator VkInstance ( ) const {
    return GetInstance( );
}
