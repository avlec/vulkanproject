#include "Wrappers.h"

void CommandBufferMgr::allocCommandBuffer(const VkDevice * device, const VkCommandPool cmdPool, VkCommandBuffer * cmdBuff, const VkCommandBufferAllocateInfo * inCommandBufferAllocateInfo)
{
	VkResult result = VK_INCOMPLETE;

	// If command information is available use it
	if (inCommandBufferAllocateInfo)
	{
		result = vkAllocateCommandBuffers(*device, inCommandBufferAllocateInfo, cmdBuff);
		assert(!result);
		return;
	}

	// Create command information since it isn't available
	VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
	commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	commandBufferAllocateInfo.pNext = nullptr;
	commandBufferAllocateInfo.commandPool = cmdPool;
	commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	commandBufferAllocateInfo.commandBufferCount = static_cast<uint32_t>(sizeof(cmdBuff) / sizeof(VkCommandBuffer));

	// Allocate the memory
	result = vkAllocateCommandBuffers(*device, &commandBufferAllocateInfo, cmdBuff);
	assert(!result);
}

void CommandBufferMgr::beginCommandBuffer(VkCommandBuffer cmdBuff, VkCommandBufferBeginInfo * inCommandBufferBeginInfo)
{
	VkResult result = VK_INCOMPLETE;


	if (inCommandBufferBeginInfo)
	{
		result = vkBeginCommandBuffer(cmdBuff, inCommandBufferBeginInfo);
		assert(result == VK_SUCCESS);
		return;
	}

	VkCommandBufferInheritanceInfo cmdBufferInheritInfo = {};
	cmdBufferInheritInfo.sType =				VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
	cmdBufferInheritInfo.pNext =				nullptr;
	cmdBufferInheritInfo.renderPass =			VK_NULL_HANDLE;
	cmdBufferInheritInfo.subpass =				NULL; // 0
	cmdBufferInheritInfo.framebuffer =			VK_NULL_HANDLE;
	cmdBufferInheritInfo.occlusionQueryEnable = VK_FALSE;
	cmdBufferInheritInfo.queryFlags =			NULL; // 0
	cmdBufferInheritInfo.pipelineStatistics =	NULL; // 0

	VkCommandBufferBeginInfo commandBufferBeginInfo = {};
	commandBufferBeginInfo.sType =					VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	commandBufferBeginInfo.pNext =					nullptr;
	commandBufferBeginInfo.flags =					NULL; // 0
	commandBufferBeginInfo.pInheritanceInfo =		&cmdBufferInheritInfo;

	result = vkBeginCommandBuffer(cmdBuff, &commandBufferBeginInfo);
	assert(result == VK_SUCCESS);
}

void CommandBufferMgr::endCommandBuffer(VkCommandBuffer cmdBuff)
{
	VkResult result = vkEndCommandBuffer(cmdBuff);
	assert(result == VK_SUCCESS);
}

void CommandBufferMgr::submitCommandBuffer(const VkQueue & queue, const VkCommandBuffer cmdBuff, const VkSubmitInfo * inSubmitInfo, const VkFence & fence)
{
	VkResult result = VK_INCOMPLETE;

	// If there isa  provided VkSubmitInfo use it
	if(inSubmitInfo)
	{
		vkQueueSubmit(queue, 1, inSubmitInfo, fence);
		result = vkQueueWaitIdle(queue);
		return;
	}

	// If none is provided use a default one.
	VkSubmitInfo submitInfo = {};
	submitInfo.sType =					VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.pNext =					nullptr;
	submitInfo.waitSemaphoreCount =		0;
	submitInfo.pWaitSemaphores =		nullptr;
	submitInfo.pWaitDstStageMask =		nullptr;
	submitInfo.commandBufferCount =		static_cast<uint32_t>(sizeof(cmdBuff) / sizeof(VkCommandBuffer));
	submitInfo.pCommandBuffers =		&cmdBuff;
	submitInfo.signalSemaphoreCount =	0;
	submitInfo.pSignalSemaphores =		nullptr;

	result = vkQueueSubmit(queue, 1, &submitInfo, fence);
	assert(!result);
	result = vkQueueWaitIdle(queue);

}
