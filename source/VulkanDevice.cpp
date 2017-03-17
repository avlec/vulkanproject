#include "..\include\VulkanDevice.h"

VulkanDevice::VulkanDevice(VkPhysicalDevice * physicalDevice)
{
	gpu = physicalDevice;
}

VulkanDevice::~VulkanDevice()
{
}

VkResult VulkanDevice::createDevice(std::vector<const char*>& layers, std::vector<const char*>& extensions)
{
	layerExtension.appRequestedLayerNames =		layers;
	layerExtension.appRequestedExtensionNames = extensions;

	VkResult result;
	float queuePriorities[1] =					{ 0.0 };

	// Create the object information
	VkDeviceQueueCreateInfo queueInfo = {};
	queueInfo.queueFamilyIndex =				graphicsQueueIndex;
	queueInfo.sType =							VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueInfo.pNext =							NULL;
	queueInfo.queueCount =						1;
	queueInfo.pQueuePriorities =				queuePriorities;

	VkDeviceCreateInfo deviceInfo = {};
	deviceInfo.sType =							VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceInfo.pNext =							NULL;
	deviceInfo.queueCreateInfoCount =			1;
	deviceInfo.pQueueCreateInfos =				&queueInfo;
	deviceInfo.enabledLayerCount =				0;
	// Device layers are deprecated
	deviceInfo.ppEnabledLayerNames =			NULL;
	deviceInfo.enabledExtensionCount =			(uint32_t) extensions.size();
	deviceInfo.ppEnabledExtensionNames =		extensions.data();
	deviceInfo.pEnabledFeatures =				NULL;

	result = vkCreateDevice(*gpu, &deviceInfo, NULL, &device);
	assert(result == VK_SUCCESS);

	return result;
}

void VulkanDevice::destroyDevice()
{
	vkDestroyDevice(device, NULL);
}

void VulkanDevice::getPhysicalDeviceQueuesAndProperties()
{
	// Query queue families count by passing NULL
	vkGetPhysicalDeviceQueueFamilyProperties(*gpu, &queueFamilyCount, NULL);
	
	// Allocate space to accomodate queue properties
	queueFamilyProperties.resize(queueFamilyCount);

	// Get queue family properties
	vkGetPhysicalDeviceQueueFamilyProperties(*gpu, &queueFamilyCount, queueFamilyProperties.data());
}

void VulkanDevice::getDeviceQueue()
{
	vkGetDeviceQueue(device, graphicsQueueWithPresentIndex, 0, &queue);
}

uint32_t VulkanDevice::getGraphicsQueueHandle()
{
	//	1. Get the number of Queues supported by the Physical device
	//	2. Get the properties each Queue type or Queue Family
	//			There could be 4 Queue type or Queue families supported by physical device - 
	//			Graphics Queue	- VK_QUEUE_GRAPHICS_BIT 
	//			Compute Queue	- VK_QUEUE_COMPUTE_BIT
	//			DMA				- VK_QUEUE_TRANSFER_BIT
	//			Sparse memory	- VK_QUEUE_SPARSE_BINDING_BIT
	//	3. Get the index ID for the required Queue family, this ID will act like a handle index to queue.

	bool found = false;

	// 1. Iterate number of queues supported by the physical device
	for (unsigned int i = 0; i < queueFamilyCount; i++) {
		// 2. Get graphics queue type
		//		There could be 4 Queue type or Queue families supported by physical device - 
		//		Graphics Queue		- VK_QUEUE_GRAPHICS_BIT 
		//		Compute Queue		- VK_QUEUE_COMPUTE_BIT
		//		DMA/Transfer Queue	- VK_QUEUE_TRANSFER_BIT
		//		Sparse memory		- VK_QUEUE_SPARSE_BINDING_BIT

		if (queueFamilyProperties[i].queueFlags && VK_QUEUE_GRAPHICS_BIT) {
			// 3. Get the handle/index ID of graphics queue family
			found = true;
			graphicsQueueIndex = i;
			break;
		}
	}
	return 0;
}
