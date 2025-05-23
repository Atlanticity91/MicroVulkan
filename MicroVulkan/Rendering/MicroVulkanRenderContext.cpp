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
MicroVulkanRenderContext::MicroVulkanRenderContext( ) 
	: FrameID{ 0 },
	ImageID{ 0 },
	Sync{ nullptr },
	Queue{ },
	CommandBuffer{ }
{ }

VkResult MicroVulkanRenderContext::CmdBeginRecord( ) {
	auto result = VK_ERROR_UNKNOWN;

	if ( CommandBuffer.GetIsValid( ) ) {
		auto specification = VkCommandBufferBeginInfo{ };

		specification.sType			   = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		specification.pNext			   = VK_NULL_HANDLE;
		specification.flags			   = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		specification.pInheritanceInfo = VK_NULL_HANDLE;

		vkResetCommandBuffer( CommandBuffer, VK_UNUSED_FLAG );

		result = vkBeginCommandBuffer( CommandBuffer, micro_ptr( specification ) );
	}

	return result;
}

bool MicroVulkanRenderContext::CmdBeginRenderPass(
	const VkRenderPassBeginInfo& render_pass_info 
) {
	return CmdBeginRenderPass( render_pass_info, VK_SUBPASS_CONTENTS_INLINE );
}

bool MicroVulkanRenderContext::CmdBeginRenderPass(
	const VkRenderPassBeginInfo& render_pass_info,
	const VkSubpassContents command_policy
) {
	const auto state = vk::IsValid( render_pass_info.renderPass ) && vk::IsValid( render_pass_info.framebuffer );

	if ( state && CommandBuffer.GetIsValid( ) )
		vkCmdBeginRenderPass( CommandBuffer, micro_ptr( render_pass_info ), command_policy );

	return state;
}

bool MicroVulkanRenderContext::CmdBeginRenderPass(
	const MicroVulkanRenderPassInfo& render_pass_info
) {
	return CmdBeginRenderPass( render_pass_info, VK_SUBPASS_CONTENTS_INLINE );
}

bool MicroVulkanRenderContext::CmdBeginRenderPass(
	const MicroVulkanRenderPassInfo& render_pass_info,
	const VkSubpassContents command_policy
) {
	const auto state = CmdBeginRenderPass( render_pass_info.BeginInfo, command_policy );

	if ( state ) {
		CmdSetViewport( render_pass_info.Viewport );
		CmdSetScissor( render_pass_info.Scissor );
	}

	return state;
}

void MicroVulkanRenderContext::CmdSetViewport( const VkViewport& viewport ) {
	vk::CmdSetViewport( CommandBuffer.Buffer, viewport );
}

void MicroVulkanRenderContext::CmdSetViewports( 
	std::initializer_list<VkViewport> viewports 
) {
	vk::CmdSetViewport( CommandBuffer.Buffer, 0, viewports );
}

void MicroVulkanRenderContext::CmdSetViewports( 
	const uint32_t start_id, 
	const std::vector<VkViewport>& viewports
) {
	vk::CmdSetViewport( CommandBuffer.Buffer, start_id, viewports );
}

void MicroVulkanRenderContext::CmdSetScissor( const VkScissor& scissor ) {
	vk::CmdSetScissor( CommandBuffer.Buffer, scissor );
}

void MicroVulkanRenderContext::CmdSetScissors( 
	std::initializer_list<VkScissor> scissors 
) {
	vk::CmdSetScissor( CommandBuffer.Buffer, 0, scissors );
}

void MicroVulkanRenderContext::CmdSetScissors(
	const uint32_t start_id,
	const std::vector<VkScissor>& scissors 
) {
	vk::CmdSetScissor( CommandBuffer.Buffer, start_id, scissors );
}

void MicroVulkanRenderContext::CmdNextSubpass( ) {
	CmdNextSubpass( VK_SUBPASS_CONTENTS_INLINE );
}

void MicroVulkanRenderContext::CmdExecute(
	const std::vector<VkCommandBuffer>& secondary_commands
) {
	const auto* buffer_data = secondary_commands.data( );
	auto command_count		= (uint32_t)secondary_commands.size( );

	if ( command_count == 0 || !CommandBuffer.GetIsValid( ) )
		return;

	vkCmdExecuteCommands( CommandBuffer, command_count, buffer_data );
}

void MicroVulkanRenderContext::CmdNextSubpass(
	const VkSubpassContents command_policy 
) {
	if ( CommandBuffer.GetIsValid( ) )
		vkCmdNextSubpass( CommandBuffer, command_policy );
}

void MicroVulkanRenderContext::CmdEndRenderPass( ) {
	if ( CommandBuffer.GetIsValid( ) )
		vkCmdEndRenderPass( CommandBuffer );
}

void MicroVulkanRenderContext::CmdEndRecord( ) {
	if ( CommandBuffer.GetIsValid( ) )
		vkEndCommandBuffer( CommandBuffer );
}
