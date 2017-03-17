#include "..\include\Headers.h"

#include "..\include\VulkanLayerAndExtension.h"
#include "..\include\VulkanApplication.h"

VulkanLayerAndExtension::VulkanLayerAndExtension()
{
}

VulkanLayerAndExtension::~VulkanLayerAndExtension()
{
}

VkResult VulkanLayerAndExtension::getInstanceLayerProperties()
{
	//Stores Number of Instance Layers
	uint32_t instanceLayerCount;
	// Vector to store layer properties
	std::vector<VkLayerProperties> layerProperties;
	// Var to check Vulkan API status
	VkResult result;

	//Query Layers
	do {
		result = vkEnumerateInstanceLayerProperties(&instanceLayerCount, NULL);

		if (result)
			return result;

		if (instanceLayerCount == 0)
			return VK_INCOMPLETE;

		layerProperties.resize(instanceLayerCount);
		result = vkEnumerateInstanceLayerProperties(&instanceLayerCount, layerProperties.data());
	} while (result == VK_INCOMPLETE);

	// Query all the extensions for each layer and store it.
	std::cout << "\nInstanced Layers" << std::endl;
	std::cout << "===================" << std::endl;
	for (auto globalLayerProp : layerProperties) {
		// Print layer name and its description
		std::cout << "\n" << globalLayerProp.description <<
			"\n\t|\n\t|---[Layer Name}---> " <<
			globalLayerProp.layerName << "\n";

		LayerProperties layerProps;
		layerProps.properties = globalLayerProp;

		// Get Instance Level Extensions for
		// corresponding layer properties
		result = getExtensionProperties(layerProps);

		if (result) {
			continue;
		}

		layerPropertyList.push_back(layerProps);


		// Print extension name for each instance layer
		for (auto j : layerProps.extensions) {
			std::cout << "\t\t|\n\t\t|---[Layer Extension]--> " << j.extensionName << "\n";
		}
	}

	return result;
}

VkResult VulkanLayerAndExtension::getExtensionProperties(LayerProperties & layerProps, VkPhysicalDevice * gpu)
{
	// # of extensions
	uint32_t extensionCount;
	// Var to check Vulkan API status
	VkResult result;

	char * layerName = layerProps.properties.layerName;
	do {
		// Fetch # of extensions on the layer
		// Use the device if one was provided otherwise use the instance
		if (gpu) {
			result = vkEnumerateDeviceExtensionProperties(*gpu, layerName, &extensionCount, NULL);
		}
		else {
			result = vkEnumerateInstanceExtensionProperties(layerName, &extensionCount, NULL);
		}
		// If there isn't anything skip to next layer
		if (result || extensionCount == 0) {
			continue;
		}

		// Gather all extension properties
		// Use the device if one was provided otherwise use the instance
		if (gpu) {
			result = vkEnumerateDeviceExtensionProperties(*gpu, layerName, &extensionCount, layerProps.extensions.data());
		}
		else {
			result = vkEnumerateInstanceExtensionProperties(layerName, &extensionCount, layerProps.extensions.data());
		}
	} while (result == VK_INCOMPLETE);

	return result;
}

VkResult VulkanLayerAndExtension::getDeviceExtensionProperties(VkPhysicalDevice * gpu)
{
	// Var to check Vulkan API status
	VkResult result;

	// Query all the extensions for each layer and store
	std::cout << "\\Device extensions" << std::endl;
	std::cout << "====================" << std::endl;
	VulkanApplication * appObj = VulkanApplication::GetInstance();
	std::vector<LayerProperties> * instanceLayerProp = &appObj->GetInstance()->instanceObj.layerExtension.layerPropertyList;

	for (auto globalLayerProp : *instanceLayerProp) {
		LayerProperties layerProps;
		layerProps.properties = globalLayerProp.properties;

		if (result = getExtensionProperties(layerProps, gpu))
			continue;

		layerPropertyList.push_back(layerProps);

		if (layerProps.extensions.size()) {
			for (auto j : layerProps.extensions) {
				std::cout << "\t\t|\n\t\t|---[Device Extension]--> " << j.extensionName << "\n";
			}
		}
		else {
			std::cout << "\t\t|\n\t\t|---[Device Extension]--> No Extension Found.\n";
		}
	}

	return result;
}
