#pragma once

#include "VulkanInstance.h"
#include "VulkanDevice.h"
#include "VulkanLayerAndExtension.h"


class VulkanApplication {
private:
	// CTOR, Application constructor responsible for layer enum
	VulkanApplication();

public:
	// DTOR
	~VulkanApplication();

private:
	// Variable for Single Ton implementation
	static std::unique_ptr<VulkanApplication> instance;
	static std::once_flag onlyOnce;

public:
	static VulkanApplication* GetInstance();

		// Simple program life cycle
	void initialize();
	void prepare();
	void update();
	void render();
	void deInitialize();

private:
	// Create the Vulkan instance object
	VkResult createVulkanInstance(std::vector<const char *> & layers, std::vector<const char *> & extensions, const char * appName);
	VkResult handShakeWithDevice(VkPhysicalDevice * gpu, std::vector<const char *> & layers, std::vector<const char *> & extensions);
	VkResult enumeratePhysicalDevices(std::vector<VkPhysicalDevice> & gpuList);

public:
	VulkanInstance instanceObj;		// Vulkan instance object

	VulkanDevice * deviceObj;
};