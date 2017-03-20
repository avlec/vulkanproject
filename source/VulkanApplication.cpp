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
	debugFlag = true;
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

void VulkanApplication::initialize()
{
	char title[] = "Hello world.";

	// Check if the supplied layers are supported or not
	if (debugFlag) {
		instanceObj.layerExtension.areLayersSupported(layerNames);
	}

	// Create the vulkan instance with specified layer extension names.
	createVulkanInstance(layerNames, instanceExtensionNames, title);

	// Create the debugging report if debugging is enabled
	if (debugFlag) {
		instanceObj.layerExtension.createDebugReportCallback();
	}

	// Get the list of physical devices on the system
	std::vector<VkPhysicalDevice> gpuList;
	enumeratePhysicalDevices(gpuList);

	// This example uses only one device which is available first
	if (gpuList.size() > 0) {
		handShakeWithDevice(&gpuList[0], layerNames, deviceExtensionNames);
	}
}

void VulkanApplication::deInitialize()
{
	deviceObj->destroyDevice();
	if (debugFlag) {
		instanceObj.layerExtension.destroyDebugReportCallback();
	}
	instanceObj.destroyInstance();
}

void VulkanApplication::prepare()
{
	// Placeholder
}

void VulkanApplication::update()
{
	// Placeholder
}

bool VulkanApplication::render()
{
	// Placeholder
	return true;
}

VkResult VulkanApplication::createVulkanInstance(std::vector<const char *> & layers, std::vector<const char *> & extensions, const char * appName)
{
	return instanceObj.createInstance(layers, extensions, appName);
}

VkResult VulkanApplication::handShakeWithDevice(VkPhysicalDevice * gpu, std::vector<const char*> & layers, std::vector<const char*> & extensions)
{
	// The user define Vulkan Device object will manage the
	// Physical and logical device and their queue properties
	deviceObj = new VulkanDevice(gpu);
	if (!deviceObj) {
		return VK_ERROR_OUT_OF_HOST_MEMORY;
	}

	// Print the devices available layer and their extension
	deviceObj->layerExtension.getDeviceExtensionProperties(gpu);

	// Get the physical device or GPU properties
	vkGetPhysicalDeviceProperties(*gpu, &deviceObj->gpuProperties);
	
	// Get the memory properties from the physical device or GPU
	vkGetPhysicalDeviceMemoryProperties(*gpu, &deviceObj->memoryProperties);

	// Query the available queues on the physical device and their properties
	deviceObj->getPhysicalDeviceQueuesAndProperties();

	// Retrieve the queue which support graphics pipeline.
	deviceObj->getGraphicsQueueHandle();

	// Create logical device, ensures that this device connects to graphics queue
	return deviceObj->createDevice(layers, extensions);
}

VkResult VulkanApplication::enumeratePhysicalDevices(std::vector<VkPhysicalDevice> & gpuList)
{
	// Holds the gpu count
	uint32_t gpuDeviceCount;
	// Var to check Vulkan API status
	VkResult result;
	// Gets the gpu count
	result = vkEnumeratePhysicalDevices(instanceObj.instance, &gpuDeviceCount, gpuList.data());
	assert(result == VK_SUCCESS);

	assert(gpuDeviceCount);

	// Make space for retrieval
	gpuList.resize(gpuDeviceCount);
	// Get physical device object
	result = vkEnumeratePhysicalDevices(instanceObj.instance, &gpuDeviceCount, gpuList.data());
	assert(result == VK_SUCCESS);

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