#pragma once

#include "Headers.h"
#include "VulkanLayerAndExtension.h"

class VulkanDevice {
public:
	VulkanDevice(VkPhysicalDevice * physicalDevice);
	~VulkanDevice();

	// Device related member variables
	VkDevice device;											// Logical Device
	VkPhysicalDevice * gpu;										// Physical Device
	VkPhysicalDeviceProperties gpuProperties;					// Physical Device attributes
	VkPhysicalDeviceMemoryProperties memoryProperties;			// Physical device memory properties

	// Queue
	VkQueue queue;												// Vulkan Queues object
	
	std::vector<VkQueueFamilyProperties> queueFamilyProperties; // Store all families exposed by the physical device

	uint32_t graphicsQueueIndex;							// Stores graphics queue index
	uint32_t graphicsQueueWithPresentIndex;						// Number of queue families exposed by device
	uint32_t queueFamilyCount;									// Device specific layer and extensions
	
	VulkanLayerAndExtension layerExtension; // Device specific extension

	VkResult createDevice(std::vector<const char *> & layers, std::vector<const char *> & extensions);
	void destroyDevice();	 
	
	void getPhysicalDeviceQueuesAndProperties();
	void getDeviceQueue();

	uint32_t getGraphicsQueueHandle();
};