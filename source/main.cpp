#include "..\include\Headers.h"

#include "VulkanLayerAndExtension.h"
#include "VulkanApplication.h"
#include "VulkanInstance.h"
#include "VulkanDevice.h"

std::vector<const char *> instanceExtensionNames = {
	VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_WIN32_SURFACE_EXTENSION_NAME
};

std::vector<const char *> layerNames = {
	"VK_LAYER_LUNARG_api_dump"
};

std::vector<const char *> deviceExtensionNames = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

int main() {
	std::cout << "vulkanproject" << std::endl << std::endl;

	VulkanApplication* appObj = VulkanApplication::GetInstance();
	appObj->initialize();
	appObj->prepare();
	appObj->render();
	appObj->deInitialize();
}

/*
void VulkanApplication::initialize()
{
	char title[] = "Hello World!";

	createVulkanInstance(layerNames, instanceExtensionNames, title);
}
*/