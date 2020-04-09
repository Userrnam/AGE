#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <set>
#include <string>

#include "utils.hpp"
#include "QueueFamilyIndicies.hpp"
#include "SwapchainSupportDetails.hpp"

#include "CoreConfig.hpp"

namespace core {

extern CoreConfig coreConfig;

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

int deviceSupportedFeatures(VkPhysicalDevice device) {
	QueueFamilyIndicies indicies = findQueueFamilies(device);

	bool extensionsSupported = checkDeviceExtensionSupport(device, coreConfig.deviceExtensions);
	
	bool swapChainAdequate = false;
	if (extensionsSupported) {
		SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}
	VkPhysicalDeviceFeatures supportedFeatures;
	vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

	// always required
	if (!indicies.isComplete() || !swapChainAdequate) {
		return -1;
	}

	int out = 0;
	if (coreConfig.features.geometryShader && supportedFeatures.geometryShader) {
		out += 1;
	}
	if (coreConfig.features.tesselationShader && supportedFeatures.tessellationShader) {
		out += 1;
	}
	if (coreConfig.features.samplerAnistropy && supportedFeatures.samplerAnisotropy) {
		out += 1;
	}
	if (coreConfig.features.sampleRateShading && supportedFeatures.sampleRateShading) {
		out += 1;
	}

	return out;
}

} // namespace core
