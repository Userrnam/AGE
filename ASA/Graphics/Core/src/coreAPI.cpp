#include <vector>
#include <string.h>
#include <iostream>

#include <GLFW/glfw3.h>

#include "coreAPI.hpp"
#include "VulkanDebug.hpp"
#include "Core.hpp"
#include "utils.hpp"
#include "QueueFamilyIndicies.hpp"


namespace core {

const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

Core apiCore;
CoreConfig coreConfig;

void debugEnable(bool b) {
	apiCore.debug.enable = b;
}

void setCoreConfig(const CoreConfig& config) {
	coreConfig = config;
}

void init(const char *appName, uint32_t appVersion) {
	glfwInit();

	if (apiCore.debug.enable && !checkValidationLayerSupport(validationLayers)) {
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
	getRequeredExtensions(extensions, apiCore.debug.enable);
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	if (apiCore.debug.enable) {
		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();

		populateDebugMessengerCreateInfo(debugCreateInfo);
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
	} else {
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateInstance(&createInfo, nullptr, &apiCore.instance) != VK_SUCCESS) {
		throw std::runtime_error("failed to create instance");
	}

	setupDebugMessenger(apiCore.instance, &apiCore.debug.messenger);
}

void pickPhysicalDevice() {
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(apiCore.instance, &deviceCount, nullptr);
	if (deviceCount == 0) {
		throw std::runtime_error("failed to find GPU with Vulkan support");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(apiCore.instance, &deviceCount, devices.data());

	VkPhysicalDevice cpu = VK_NULL_HANDLE;
	VkPhysicalDevice bestDevice = VK_NULL_HANDLE;
	uint32_t maxSuppotredFeatues = 0;
	for (const auto& device : devices) {
		uint32_t supportedFeatures = deviceSupportedFeatures(device);
		if (supportedFeatures != -1) {
			VkPhysicalDeviceProperties deviceProperties;
			vkGetPhysicalDeviceProperties(device, &deviceProperties);

			// ignore cpu
			if (deviceProperties.deviceType & VK_PHYSICAL_DEVICE_TYPE_CPU) {
				cpu = device;
				continue;
			}

			apiCore.physicalDevice = device;

			if (deviceProperties.deviceType & VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
				if (supportedFeatures > maxSuppotredFeatues) {
					bestDevice = device;
					maxSuppotredFeatues = supportedFeatures;
				}
			}
			break;
		}
	}

	// only cpu available
	if (apiCore.physicalDevice == VK_NULL_HANDLE) {
		std::cerr << "warning: only cpu available\n";
		apiCore.physicalDevice = cpu;
	}

	if (bestDevice) {
		apiCore.physicalDevice = bestDevice;
	}

	// update features
	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceFeatures(apiCore.physicalDevice, &deviceFeatures);
	
	if (coreConfig.features.geometryShader && deviceFeatures.geometryShader) {
		std::cerr << "warning: geometry shader unavailable\n";
	}
	if (coreConfig.features.tesselationShader && deviceFeatures.tessellationShader) {
		std::cerr << "warning: tesselation shader unavailable\n";
	}
	if (coreConfig.features.samplerAnistropy && deviceFeatures.samplerAnisotropy) {
		std::cerr << "warning: sampler anistropy unavailable\n";
	}
	if (coreConfig.features.sampleRateShading && deviceFeatures.sampleRateShading) {
		std::cerr << "warning: sample rate shading unavailable\n";
	}

	if (apiCore.physicalDevice == VK_NULL_HANDLE) {
		throw std::runtime_error("failed to find suitable GPU");
	}
}

void createLogicalDevice() {
	// QueueFamilyIndicies indicies = findQueueFamilies(physicalDevice);

	// std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	// std::set<uint32_t> uniqueQueueFamilies = { indicies.graphicsFamily.value(), indicies.presentFamily.value() };

	// float queuePriority = 1.0f;
	// for (uint32_t queueFamily : uniqueQueueFamilies) {
	// 	VkDeviceQueueCreateInfo queueCreateInfo = {};
	// 	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	// 	queueCreateInfo.queueFamilyIndex = queueFamily;
	// 	queueCreateInfo.queueCount = 1;
	// 	queueCreateInfo.pQueuePriorities = &queuePriority;
	// 	queueCreateInfos.push_back(queueCreateInfo);
	// }

	// VkPhysicalDeviceFeatures deviceFeatures = {};
	// deviceFeatures.samplerAnisotropy = VK_TRUE;
	// deviceFeatures.sampleRateShading = VK_TRUE;

	// VkDeviceCreateInfo createInfo = {};
	// createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	
	// createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	// createInfo.pQueueCreateInfos = queueCreateInfos.data();

	// createInfo.pEnabledFeatures = &deviceFeatures;

	// createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	// createInfo.ppEnabledExtensionNames = deviceExtensions.data();

	// if (enableValidationLayers) {
	// 	createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
	// 	createInfo.ppEnabledLayerNames = validationLayers.data();
	// } else {
	// 	createInfo.enabledLayerCount = 0;
	// }

	// if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
	// 	throw std::runtime_error("failed to create logical device");
	// }

	// vkGetDeviceQueue(device, indicies.graphicsFamily.value(), 0, &graphicsQueue);
	// vkGetDeviceQueue(device, indicies.presentFamily.value(), 0, &presentQueue);
}

void destroy() {
	if (apiCore.debug.enable) {
		destroyDebugUtilsMessengerEXT(apiCore.instance, apiCore.debug.messenger);
	}

	vkDestroySurfaceKHR(apiCore.instance, apiCore.window.surface, nullptr);
	vkDestroyInstance(apiCore.instance, nullptr);

	glfwDestroyWindow(apiCore.window.handle);
	glfwTerminate();
}

} // namespace core
