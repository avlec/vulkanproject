#pragma once

#include <vulkan\vulkan.h>

#include "Headers.h"

struct LayerProperties {
	VkLayerProperties					properties;
	std::vector<VkExtensionProperties>	extensions;
};

class VulkanLayerAndExtension {
public:
	VulkanLayerAndExtension();
	~VulkanLayerAndExtension();

	std::vector<const char *> appRequestedLayerNames;

	std::vector<const char *> appRequestedExtensionNames;

	// Layers and corresponding extension list
	std::vector<LayerProperties> layerPropertyList;

	// Instance/global layer
	VkResult getInstanceLayerProperties();

	// Global extensions
	VkResult getExtensionProperties(LayerProperties &layerProps, VkPhysicalDevice * gpu = NULL);

	// Device based extensions
	VkResult getDeviceExtensionProperties(VkPhysicalDevice * gpu);
};