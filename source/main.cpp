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

int main() {
	std::cout << "3_0_DeviceHandshake" << std::endl << std::endl;

	VulkanApplication* appObj = VulkanApplication::GetInstance();
	appObj->initialize();
	while (1) {
		continue;
	}
}

void VulkanApplication::initialize()
{
	char title[] = "Hello World!";

	createVulkanInstance(layerNames, instanceExtensionNames, title);
}