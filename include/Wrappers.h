#pragma once
#include "Headers.h"

class CommandBufferMgr
{
public:
	// Allocate memory for command buffers from the command pool
	static void allocCommandBuffer(const VkDevice * device, const VkCommandPool cmdPool, VkCommandBuffer * cmdBuff, const VkCommandBufferAllocateInfo * inCommandBufferAllocateInfo);
	// Start command buffer recording
	static void beginCommandBuffer(VkCommandBuffer cmdBuff, VkCommandBufferBeginInfo * inCommandBufferBeginInfo = nullptr);
	// End the command buffer recording
	static void endCommandBuffer(VkCommandBuffer cmdBuff);
	// Submit the command buffer
	static void submitCommandBuffer(const VkQueue & queue, const VkCommandBuffer cmdBuff, const VkSubmitInfo * inSubmitInfo = nullptr, const VkFence & fence = nullptr);
};
