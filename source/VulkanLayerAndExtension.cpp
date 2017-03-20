#include "..\include\Headers.h"

#include "..\include\VulkanLayerAndExtension.h"
#include "..\include\VulkanApplication.h"

VulkanLayerAndExtension::VulkanLayerAndExtension()
{
	dbgCreateDebugReportCallback =	NULL;
	dbgDestroyDebugReportCallback = NULL;
	debugReportCallback =			NULL;
}

VulkanLayerAndExtension::~VulkanLayerAndExtension()
{
	dbgCreateDebugReportCallback =	NULL;
	dbgDestroyDebugReportCallback = NULL;
	debugReportCallback =			NULL;
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

		std::cout << "\n" << globalLayerProp.properties.description << "\n\t|\n\t|---[Layer Name]--> " << globalLayerProp.properties.layerName << "\n";
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

VkBool32 VulkanLayerAndExtension::areLayersSupported(std::vector<const char*>& layerNames)
{
	uint32_t checkCount = (uint32_t) layerNames.size();
	uint32_t layerCount = (uint32_t) layerPropertyList.size();
	std::vector<const char *> unsupportedLayerNames;
	// itterate through the list of layernames
	for (uint32_t i = 0; i < checkCount; i++) {
		VkBool32 isSupported = 0;
		for (uint32_t j = 0; j < layerCount; j++) {
			// Mark if it has been found in both lists
			if (!strcmp(layerNames[i], layerPropertyList[j].properties.layerName)) {
				isSupported = 1;
			}
		}

		// If it isn't supported add it to the list of things to be erased from layerNames
		// and print out a bad message
		if (!isSupported) {
			std::cout << "No layer support found, removed from layer: " << layerNames[i] << std::endl;
			unsupportedLayerNames.push_back(layerNames[i]);
		}
		// Otherwise print out a nice message
		else {
			std::cout << "Layer supported: " << layerNames[i] << std::endl;
		}
	}

	// Itterate through the list of unsupportedLayerNames and erase them from layerNames
	for (auto i : unsupportedLayerNames) {
		auto it = std::find(layerNames.begin(), layerNames.end(), i);
		if (it != layerNames.end()) {
			layerNames.erase(it);
		}
	}

	return true;
}

VKAPI_ATTR VkBool32 VKAPI_CALL
VulkanLayerAndExtension::debugFunction(VkDebugReportFlagsEXT msgFlags, VkDebugReportObjectTypeEXT objType, uint64_t srcObject, size_t location, uint32_t msgCode, const char * layerPrefix, const char * msg, void * userData)
{
	std::cout << "[VK_DEBUG_REPORT] ";
	if (msgFlags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
		std::cout << "ERROR";
	}
	else if (msgFlags & VK_DEBUG_REPORT_WARNING_BIT_EXT) {
		std::cout << "WARNING";
	}
	else if (msgFlags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT) {
		std::cout << "INFORMATION";
	}
	else if (msgFlags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) {
		std::cout << "PERFORMANCE";
	}
	else if (msgFlags & VK_DEBUG_REPORT_DEBUG_BIT_EXT) {
		std::cout << "DEBUG";
	}
	else {
		return VK_FALSE;
	}
	std::cout << ": [" << layerPrefix << "] Code" << msgCode << ":" << msg << std::endl;
	return VK_TRUE;
}

VkResult VulkanLayerAndExtension::createDebugReportCallback()
{
	VkResult result;

	VulkanApplication * appObj = VulkanApplication::GetInstance();
	VkInstance* instance = &appObj->instanceObj.instance;

	// Get vkCreateDebugReportCallbackEXT API
	dbgCreateDebugReportCallback = (PFN_vkCreateDebugReportCallbackEXT) vkGetInstanceProcAddr(*instance, "vkCreateDebugReportCallbackEXT");
	if (!dbgCreateDebugReportCallback) {
		std::cout << "Error: GetInstanceProcAddr unable to locate vkCreateDebugReportCallbackEXT function.\n";
		return VK_ERROR_INITIALIZATION_FAILED;
	}
	std::cout << "GetInstanceProcAddr loaded dbgCreateDebugReportCallback function.\n";

	// Get vkDestroyDebugReportCallbackEXT API
	dbgDestroyDebugReportCallback = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(*instance, "vkDestroyDebugReportCallbackEXT");
	if (!dbgDestroyDebugReportCallback) {
		std::cout << "Error: GetInstanceProcAddr unable to locate vkDestroyDebugReportCallbackEXT function.\n";
		return VK_ERROR_INITIALIZATION_FAILED;
	}
	std::cout << "GetInstanceProcAddr loaded dbgDestroyDebugReportCallback function.\n";

	dbgReportCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;

	/*
	Only error, have no idea why it pops up cause typedef in vulkan.h should allow for it to be recognized

	---- Snippets from vulkan/vulkan.h -------------------------------------------

		typdef uint32_t VkBool32;

		typedef VkBool32 (VKAPI_PTR *PFN_vkDebugReportCallbackEXT)(
			VkDebugReportFlagsEXT                       flags,
			VkDebugReportObjectTypeEXT                  objectType,
			uint64_t                                    object,
			size_t                                      location,
			int32_t                                     messageCode,
			const char*                                 pLayerPrefix,
			const char*                                 pMessage,
			void*                                       pUserData);
		

		typedef struct VkDebugReportCallbackCreateInfoEXT {
			VkStructureType                 sType;
			const void*                     pNext;
			VkDebugReportFlagsEXT           flags;
			PFN_vkDebugReportCallbackEXT    pfnCallback;
			void*                           pUserData;
		} VkDebugReportCallbackCreateInfoEXT;

	----- Attempted Fixes --------------------------------------------------------

		dbgReportCreateInfo.pfnCallback = (PFN_vkDebugReportCallbackEXT) debugFunction;
			
			and

		dbgReportCreateInfo.pfnCallback = reinterpret_cast<PFN_vkDebugReportCallbackEXT>(debugFunction);

		Both compile but cause a memory access violation, because typecasting fails and the Vulkan API attempts to access a broken function pointer

	---- Error Message -----------------------------------------------------------

	Severity	Code	Description	Project	File	Line	Source	Suppression State
	Error	C2440	'=': cannot convert from 'VkBool32 (__cdecl *)(VkDebugReportFlagsEXT,VkDebugReportObjectTypeEXT,uint64_t,std::size_t,uint32_t,const char *,const char *,void *)'
	to 'PFN_vkDebugReportCallbackEXT'	vulkanproject	C:\Users\Alec\workspace\cpp\vulkanproject\source\VulkanLayerAndExtension.cpp	231	Build

	------------------------------------------------------------------------------
	*/
	dbgReportCreateInfo.pfnCallback =   VulkanLayerAndExtension::debugFunction;
	dbgReportCreateInfo.pUserData =		NULL;
	dbgReportCreateInfo.pNext =			NULL;
	dbgReportCreateInfo.flags =			VK_DEBUG_REPORT_WARNING_BIT_EXT |
										VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
										VK_DEBUG_REPORT_ERROR_BIT_EXT |
										VK_DEBUG_REPORT_DEBUG_BIT_EXT;

	// Create the debug report callback object
	result = dbgCreateDebugReportCallback(*instance, &dbgReportCreateInfo, NULL, &debugReportCallback);
	if (result == VK_SUCCESS) {
		std::cout << "Debug report callback object created successfully.\n";
	}

	return result;
}

void VulkanLayerAndExtension::destroyDebugReportCallback()
{
	VulkanApplication * appObj = VulkanApplication::GetInstance();
	VkInstance & instance = appObj->instanceObj.instance;
	if (debugReportCallback) {
		dbgDestroyDebugReportCallback(instance, debugReportCallback, NULL);
	}
}
