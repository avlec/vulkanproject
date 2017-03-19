#pragma once

#include "Headers.h"
#include "VulkanLayerAndExtension.h"

class VulkanInstance {
public:
	VulkanInstance();
	~VulkanInstance();

	// Vulkan instance object variable
	VkInstance instance;

	// Vulkan instance specific layer and extensions
	VulkanLayerAndExtension layerExtension;
	
	// Functions for creation and deletion of vulkan instance
	VkResult createInstance(std::vector<const char *> & layers,
							std::vector<const char *> & extensions,
							const char * appName);

	// Destroy Vulkan Instance
	void destroyInstance();
};