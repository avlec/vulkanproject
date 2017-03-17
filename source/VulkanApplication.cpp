#include "..\include\VulkanApplication.h"

std::unique_ptr<VulkanApplication> VulkanApplication::instance;
std::once_flag VulkanApplication::onlyOnce;

extern std::vector<const char *> instanceExtensionNames;
extern std::vector<const char *> layerNames;
extern std::vector<const char *> deviceExtensionNames;

VulkanApplication::VulkanApplication()
{
	instanceObj.layerExtension.getInstanceLayerProperties();

	deviceObj = NULL;
}


VulkanApplication::~VulkanApplication()
{
}

// Returns the Single ton object of VulkanApplication
VulkanApplication* VulkanApplication::GetInstance()
{
	std::call_once(onlyOnce, []() {instance.reset(new VulkanApplication()); });
	return instance.get();
}

VkResult VulkanApplication::createVulkanInstance(std::vector<const char *> & layers, std::vector<const char *> & extensions, const char * appName)
{
	return instanceObj.createInstance(layers, extensions, appName);
}

VkResult VulkanApplication::handShakeWithDevice(VkPhysicalDevice * gpu, std::vector<const char*> & layers, std::vector<const char*> & extensions)
{
	return VkResult();
}

VkResult VulkanApplication::enumeratePhysicalDevices(std::vector<VkPhysicalDevice> & gpuList)
{
	// Holds the gpu count
	uint32_t gpuDeviceCount;
	// Var to check Vulkan API status
	VkResult result;
	// Gets the gpu count
	result = vkEnumeratePhysicalDevices(instanceObj.instance, &gpuDeviceCount, gpuList.data());

	// Make space for retrieval
	gpuList.resize(gpuDeviceCount);
	// Get physical device object
	result = vkEnumeratePhysicalDevices(instanceObj.instance, &gpuDeviceCount, gpuList.data());

	return result;
}

// This function is responsible for creating the logical device.
// The process of logical device creation requires the following steps:-
// 1. Get the physical device specific layer and corresponding extensions [Optional]
// 2. Create user define VulkanDevice object
// 3. Provide the list of layer and extension that needs to enabled in this physical device
// 4. Get the physical device or GPU properties
// 5. Get the memory properties from the physical device or GPU
// 6. Query the physical device exposed queues and related properties
// 7. Get the handle of graphics queue
// 8. Create the logical device, connect it to the graphics queue.

// High level function for creating device and queues