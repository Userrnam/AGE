#include <vector>
#include <string.h>

#include "core/AppProperties.hpp"
#include "core/VulkanDebug.hpp"

const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

namespace core {

void AppProperties::createInstance(const char *appName, uint32_t appVersion) {
	if (debugEnable && !checkValidationLayerSupport(validationLayers)) {
		throw std::runtime_error("requested unavailable validation layers");
	}

	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = appName;
	appInfo.applicationVersion = appVersion;
	appInfo.pEngineName = "ASA";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	if (debugEnable) {
		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();

		populateDebugMessengerCreateInfo(debugCreateInfo);
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
	} else {
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
		throw std::runtime_error("failed to create instance");
	}

	setupDebugMessenger(instance, &debugMessenger);
}

void AppProperties::destroy() {
	if (debugEnable) {
		destroyDebugUtilsMessengerEXT(instance, debugMessenger);
	}
	vkDestroyInstance(instance, nullptr);
}

} // namespace core
