#include "..\include\VulkanInstance.h"

VulkanInstance::VulkanInstance():
instance(nullptr)
{
}

VulkanInstance::~VulkanInstance()
{
}

VkResult VulkanInstance::createInstance(std::vector<const char*>& layers, std::vector<const char*>& extensions, const char * appName)
{
	layerExtension.appRequestedExtensionNames = extensions;
	layerExtension.appRequestedLayerNames =		layers;

	// Define the Vulkan app structure
	VkApplicationInfo appInfo = {};
	appInfo.sType =								VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pNext = nullptr;
	appInfo.pApplicationName =					appName;
	appInfo.applicationVersion =				1;
	appInfo.pEngineName =						appName;
	appInfo.engineVersion =						1;
	appInfo.apiVersion =						VK_MAKE_VERSION(1, 0, 0);

	// Define the Vulkan instance create info structure
	VkInstanceCreateInfo instInfo = {};
	instInfo.sType =							VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instInfo.pNext =							&layerExtension.dbgReportCreateInfo;
	instInfo.flags =							0;
	instInfo.pApplicationInfo =					&appInfo;

	// Specify the list of layer name to be enabled
	instInfo.enabledLayerCount =				static_cast<uint32_t>(layers.size());
	instInfo.ppEnabledLayerNames =				layers.size() ? layers.data() : NULL;

	// Specify the list of extensions to be used in the app
	instInfo.enabledExtensionCount =			static_cast<uint32_t>(extensions.size());
	instInfo.ppEnabledExtensionNames =			extensions.size() ? extensions.data() : NULL;
	
	// Create the instance
	VkResult result = vkCreateInstance(&instInfo, nullptr, &instance);
	assert(result == VK_SUCCESS);

	return result;
}

// ReSharper disable once CppMemberFunctionMayBeConst
void VulkanInstance::destroyInstance()
{
	vkDestroyInstance(instance, nullptr);
}
