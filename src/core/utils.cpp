#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <set>
#include <string>

#include "core/utils.hpp"
#include "core/QueueFamilyIndicies.hpp"
#include "core/SwapchainSupportDetails.hpp"

namespace core {

void getRequeredExtensions(std::vector<const char*>& extensions, bool enableValidationLayers) {
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;

	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	for (int i = 0; i < glfwExtensionCount; ++i) {
		extensions.push_back(glfwExtensions[i]);
	}

	if (enableValidationLayers) {
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}
}

bool checkDeviceExtensionSupport(const VkPhysicalDevice device, const std::vector<const char*>& deviceExtensions) {
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
	std::vector<VkExtensionProperties> availableExtansions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtansions.data());
	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
	for (const auto& extension : availableExtansions) {
		requiredExtensions.erase(extension.extensionName);
	}
	return requiredExtensions.empty();
}

bool isDeviceSuitable(VkPhysicalDevice device, const DeviceRequirements& requirements) {
	QueueFamilyIndicies indicies = findQueueFamilies(device);

	bool extensionsSupported = checkDeviceExtensionSupport(device, requirements.deviceExtensions);
	
	bool swapChainAdequate = false;
	if (extensionsSupported) {
		SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}
	VkPhysicalDeviceFeatures supportedFeatures;
	vkGetPhysicalDeviceFeatures(device, &supportedFeatures);
    // FIXME: 
	return indicies.isComplete() && swapChainAdequate && supportedFeatures.samplerAnisotropy;
}

} // namespace core
