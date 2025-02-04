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
            CreateDebugMessenger( )  &&
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
    return window.CreateSurface( m_instance, m_surface );
}

////////////////////////////////////////////////////////////////////////////////////////////
//		===	PRIVATE STATIC ===
////////////////////////////////////////////////////////////////////////////////////////////
VkBool32 MicroVulkanInstance::DebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData
) {
    micro_unused( pUserData );

    auto message = std::string{ "" };

    /*
    message += vk::to_string( static_cast<vk::DebugUtilsMessageSeverityFlagBitsEXT>( messageSeverity ) ) + ": " +
        vk::to_string( static_cast<vk::DebugUtilsMessageTypeFlagsEXT>( messageTypes ) ) + ":\n";
    message += std::string( "\t" ) + "messageIDName   = <" + pCallbackData->pMessageIdName + ">\n";
    message += std::string( "\t" ) + "messageIdNumber = " + std::to_string( pCallbackData->messageIdNumber ) + "\n";
    message += std::string( "\t" ) + "message         = <" + pCallbackData->pMessage + ">\n";
    if ( 0 < pCallbackData->queueLabelCount ) {
        message += std::string( "\t" ) + "Queue Labels:\n";
        for ( uint32_t i = 0; i < pCallbackData->queueLabelCount; i++ ) {
            message += std::string( "\t\t" ) + "labelName = <" + pCallbackData->pQueueLabels[ i ].pLabelName + ">\n";
        }
    }
    if ( 0 < pCallbackData->cmdBufLabelCount ) {
        message += std::string( "\t" ) + "CommandBuffer Labels:\n";
        for ( uint32_t i = 0; i < pCallbackData->cmdBufLabelCount; i++ ) {
            message += std::string( "\t\t" ) + "labelName = <" + pCallbackData->pCmdBufLabels[ i ].pLabelName + ">\n";
        }
    }
    if ( 0 < pCallbackData->objectCount ) {
        for ( uint32_t i = 0; i < pCallbackData->objectCount; i++ ) {
            message += std::string( "\t" ) + "Object " + std::to_string( i ) + "\n";
            message += std::string( "\t\t" ) + "objectType   = " + vk::to_string( static_cast<vk::ObjectType>( pCallbackData->pObjects[ i ].objectType ) ) + "\n";
            message += std::string( "\t\t" ) + "objectHandle = " + std::to_string( pCallbackData->pObjects[ i ].objectHandle ) + "\n";
            if ( pCallbackData->pObjects[ i ].pObjectName ) {
                message += std::string( "\t\t" ) + "objectName   = <" + pCallbackData->pObjects[ i ].pObjectName + ">\n";
            }
        }
    }
    */

    #ifdef _WIN32
    _CrtDbgReport( _CRT_ASSERT, "Vulkan", pCallbackData->messageIdNumber, pCallbackData->pMessageIdName, pCallbackData->pMessage );
    #else
    printf( "%s\n", message.c_str( ) );
    #endif

    return VK_FALSE;
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
