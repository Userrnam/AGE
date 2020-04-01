#include <vector>
#include <string.h>

#include <GLFW/glfw3.h>

#include "core/AppProperties.hpp"
#include "core/VulkanDebug.hpp"

const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

void getRequeredExtensions(std::vector<const char*>& extensions, bool enableValidationLayers) {
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;

	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	extensions.resize(glfwExtensionCount+1);
	for (int i = 0; i < glfwExtensionCount; ++i) {
		extensions.push_back(glfwExtensions[i]);
	}

	if (enableValidationLayers) {
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}
}

namespace core {

void AppProperties::debugEnable(bool b) {
	m_debugEnable = b;
}

void AppProperties::createInstance(const char *appName, uint32_t appVersion) {
	if (m_debugEnable && !checkValidationLayerSupport(validationLayers)) {
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

	std::vector<const char*> extensions;
	getRequeredExtensions(extensions, m_debugEnable);
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	if (m_debugEnable) {
		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();

		populateDebugMessengerCreateInfo(debugCreateInfo);
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
	} else {
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS) {
		throw std::runtime_error("failed to create instance");
	}

	setupDebugMessenger(m_instance, &m_debugMessenger);
}

void AppProperties::destroy() {
	if (m_debugEnable) {
		destroyDebugUtilsMessengerEXT(m_instance, m_debugMessenger);
	}
	vkDestroyInstance(m_instance, nullptr);
}

} // namespace core
