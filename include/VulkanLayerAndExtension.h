#pragma once

#include "Headers.h"

struct LayerProperties {
	VkLayerProperties					properties;
	std::vector<VkExtensionProperties>	extensions;
};

class VulkanLayerAndExtension {
public:
	VulkanLayerAndExtension();
	~VulkanLayerAndExtension();

	// Layer names requested by the application
	std::vector<const char *> appRequestedLayerNames;

	// Extension names requested by the application
	std::vector<const char *> appRequestedExtensionNames;

	// Layers and corresponding extension list
	std::vector<LayerProperties> layerPropertyList;

	// Instance/global layer
	VkResult getInstanceLayerProperties();

	// Global extensions
	VkResult getExtensionProperties(LayerProperties &layerProps, VkPhysicalDevice * gpu = NULL);

	// Device based extensions
	VkResult getDeviceExtensionProperties(VkPhysicalDevice * gpu);


	/*** Vulkan debugging member functions and variables ***/

	VkBool32 areLayersSupported(std::vector<const char *> & layerNames);
	VkResult createDebugReportCallback();
	void destroyDebugReportCallback();
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugFunction(VkDebugReportFlagsEXT msgFlags,
		VkDebugReportObjectTypeEXT objType,
		uint64_t srcObject,
		size_t location,
		uint32_t msgCode,
		const char * layerPrefix,
		const char * msg,
		void * userData);

private:
	PFN_vkCreateDebugReportCallbackEXT dbgCreateDebugReportCallback;
	PFN_vkDestroyDebugReportCallbackEXT dbgDestroyDebugReportCallback;
	VkDebugReportCallbackEXT debugReportCallback;
public:
	VkDebugReportCallbackCreateInfoEXT dbgReportCreateInfo = {};
};