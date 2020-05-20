#include <vector>
#include <string.h>
#include <iostream>
#include <set>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "coreAPI.hpp"
#include "VulkanDebug.hpp"
#include "Core.hpp"
#include "utils.hpp"
#include "QueueFamilyIndicies.hpp"
#include "SwapchainSupportDetails.hpp"


namespace age::core {

const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

Core apiCore;
CoreConfig coreConfig;

void setCoreConfig(const CoreConfig& config) {
	coreConfig = config;
	apiCore.debug.enable = config.debugEnable;
}

void init() {
	glfwInit();

	if (apiCore.debug.enable && !checkValidationLayerSupport(validationLayers)) {
		throw std::runtime_error("requested unavailable validation layers");
	}

	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = coreConfig.appInfo.name.c_str();
	appInfo.applicationVersion = coreConfig.appInfo.version;
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

	if (apiCore.debug.enable) {
		setupDebugMessenger(apiCore.instance, &apiCore.debug.messenger);
	}
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
	
	if (coreConfig.features.geometryShader && !deviceFeatures.geometryShader) {
		std::cerr << "warning: geometry shader unavailable\n";
		coreConfig.features.geometryShader = false;
	}
	if (coreConfig.features.tesselationShader && !deviceFeatures.tessellationShader) {
		std::cerr << "warning: tesselation shader unavailable\n";
		coreConfig.features.tesselationShader = false;
	}
	if (coreConfig.features.samplerAnistropy && !deviceFeatures.samplerAnisotropy) {
		std::cerr << "warning: sampler anistropy unavailable\n";
		coreConfig.features.samplerAnistropy = false;
	}
	if (coreConfig.features.sampleRateShading && !deviceFeatures.sampleRateShading) {
		std::cerr << "warning: sample rate shading unavailable\n";
		coreConfig.features.sampleRateShading = false;
	}

	if (apiCore.physicalDevice == VK_NULL_HANDLE) {
		throw std::runtime_error("failed to find suitable GPU");
	}

	// set multisampling
	VkSampleCountFlagBits maxSamples = getMaxSampleCount();
	if (coreConfig.multisampling.sampleCount > maxSamples) {
		std::cout << "warning: sample count reduced to " << maxSamples << std::endl;
		coreConfig.multisampling.sampleCount = maxSamples;
	}
	apiCore.multisampling.sampleCount = coreConfig.multisampling.sampleCount;

	apiCore.depth.format = findDepthFormat();
}

// FIXME: add queue choice
// Default: 1 graphicsQueue, 1 presentQueue, 1 transferQueue
void createLogicalDevice() {
	std::vector<VkQueueFamilyProperties> queues = getQueueFamilyProperties(apiCore.physicalDevice);

	bool graphicsQueueFound = false;
	bool presentQueueFound = false;
	bool transferQueueFound = false;

	uint32_t presentQueueIndex = 0;

	// try to find queue that supports only transfer
	uint32_t index = 0;
	for (auto& queue : queues) {
		if (!transferQueueFound && queue.queueFlags == VK_QUEUE_TRANSFER_BIT) {
			transferQueueFound = true;
			apiCore.queues.transfer.index = index;
		}
		index++;
	}

	index = 0;
	for (auto &queue : queues) {
		// skip transfer only queue
		if (transferQueueFound && apiCore.queues.transfer.index == index) {
			index++;
			continue;
		}
		if (!graphicsQueueFound && (queue.queueFlags & VK_QUEUE_GRAPHICS_BIT)) {
			apiCore.queues.graphics.index = index;
			graphicsQueueFound = true;
			if (!presentQueueFound && (queue.queueCount > 1)) {
				VkBool32 presentSupport = false;
				vkGetPhysicalDeviceSurfaceSupportKHR(apiCore.physicalDevice, index, apiCore.window.surface, &presentSupport);

				if (presentSupport) {
					apiCore.queues.present.index = index;
					presentQueueFound = true;
					presentQueueIndex = 1;
				}
			}
		} else if (!transferQueueFound && (queue.queueFlags & VK_QUEUE_TRANSFER_BIT)) {
			apiCore.queues.transfer.index = index;
			transferQueueFound = true;
			if (!presentQueueFound && (queue.queueCount > 1)) {
				VkBool32 presentSupport = false;
				vkGetPhysicalDeviceSurfaceSupportKHR(apiCore.physicalDevice, index, apiCore.window.surface, &presentSupport);

				if (presentSupport) {
					apiCore.queues.present.index = index;
					presentQueueFound = true;
					presentQueueIndex = 1;
				}
			}
		} else if (!presentQueueFound) {
			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(apiCore.physicalDevice, index, apiCore.window.surface, &presentSupport);
			if (presentSupport) {
				apiCore.queues.present.index = index;
				presentQueueFound = true;
				presentQueueIndex = 0;
			}
		}
		index++;
	}

	if (!graphicsQueueFound) {
		throw std::runtime_error("graphics queue not found");
	}
	
	std::vector<uint32_t> queueFamilies = {
		apiCore.queues.graphics.index,
		apiCore.queues.transfer.index,
		apiCore.queues.present.index
	};

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	float queuePriority = 1.0f;
	for (uint32_t queueFamily : queueFamilies) {
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceFeatures deviceFeatures = {};
	deviceFeatures.samplerAnisotropy = VK_TRUE;
	deviceFeatures.sampleRateShading = VK_TRUE;

	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();

	createInfo.pEnabledFeatures = &deviceFeatures;

	createInfo.enabledExtensionCount = static_cast<uint32_t>(coreConfig.deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = coreConfig.deviceExtensions.data();

	if (apiCore.debug.enable) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	} else {
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(apiCore.physicalDevice, &createInfo, nullptr, &apiCore.device) != VK_SUCCESS) {
		throw std::runtime_error("failed to create logical device");
	}

	vkGetDeviceQueue(apiCore.device, apiCore.queues.graphics.index, 0, &apiCore.queues.graphics.queue);
	vkGetDeviceQueue(apiCore.device, apiCore.queues.transfer.index, 0, &apiCore.queues.transfer.queue);
	vkGetDeviceQueue(apiCore.device, apiCore.queues.present.index, presentQueueIndex, &apiCore.queues.present.queue);
}

// FIXME: choose surface format
void createSwapchain() {
	SwapChainSupportDetails swapChainSupport = querySwapChainSupport(apiCore.physicalDevice);

	VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
	VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

	uint32_t imageCount = swapChainSupport.capabilities.minImageCount;
	if (swapChainSupport.capabilities.minImageCount <= 3) {
		if (swapChainSupport.capabilities.maxImageCount > 0) {
			if (3 <= swapChainSupport.capabilities.maxImageCount) {
				imageCount = 3;
			} else {
				imageCount = swapChainSupport.capabilities.maxImageCount;
			}
		} else {
			imageCount = 3;
		}
	}

	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = apiCore.window.surface;

	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;								// need for vkClearColorImage
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;

// FIXME:
	// QueueFamilyIndicies indicies = findQueueFamilies(apiCore.physicalDevice);
	// uint32_t queueFamilyIndicies[] = { indicies.graphicsFamily.value(), indicies.presentFamily.value() };

	// if (indicies.graphicsFamily != indicies.presentFamily) {
	// 	createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
	// 	createInfo.queueFamilyIndexCount = 2;
	// 	createInfo.pQueueFamilyIndices = queueFamilyIndicies;
	// } else {
	//  createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	// 	createInfo.queueFamilyIndexCount = 0;           // optional
	// 	createInfo.pQueueFamilyIndices = nullptr;       // optional
	// }

	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;

	createInfo.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(apiCore.device, &createInfo, nullptr, &apiCore.swapchain.swapchain) != VK_SUCCESS) {
		throw std::runtime_error("filed to create swap chain");
	}
	
	vkGetSwapchainImagesKHR(apiCore.device, apiCore.swapchain.swapchain, &imageCount, nullptr);
	apiCore.swapchain.images.resize(imageCount);
	vkGetSwapchainImagesKHR(apiCore.device, apiCore.swapchain.swapchain, &imageCount, apiCore.swapchain.images.data());

	apiCore.swapchain.format = surfaceFormat.format;
	apiCore.swapchain.extent = extent;

	// create image views
	apiCore.swapchain.imageViews.resize(imageCount);
	for (size_t i = 0; i < imageCount; ++i) {
		apiCore.swapchain.imageViews[i] = createImageView(
			apiCore.swapchain.images[i],
			apiCore.swapchain.format,
			VK_IMAGE_ASPECT_COLOR_BIT,
			1
		);
	}
}

void createDepthResources() {
	apiCore.depth.image.create(
		ImageCreateInfo()
			.setFormat(findDepthFormat())
			.setExtent(apiCore.swapchain.extent)
			.setMipLevel(1)
			.setSampleCount(apiCore.multisampling.sampleCount)
			.setImageUsage(VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT)
			.setMemoryProperties(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
			.setAspectFlags(VK_IMAGE_ASPECT_DEPTH_BIT)
	);
}

void createMultisamplingResources() {
	if (apiCore.multisampling.sampleCount == VK_SAMPLE_COUNT_1_BIT) {
		return;
	}

	apiCore.multisampling.image.create(
		ImageCreateInfo()
			.setFormat(apiCore.swapchain.format)
			.setExtent(apiCore.swapchain.extent)
			.setMipLevel(1)
			.setSampleCount(apiCore.multisampling.sampleCount)
			.setImageUsage(VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT)
			.setMemoryProperties(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
			.setAspectFlags(VK_IMAGE_ASPECT_COLOR_BIT)
	);
}

// FIXME: add compute
void createCommandPools() {
	VkCommandPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = apiCore.queues.graphics.index;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

	// create graphics command pool
	if (vkCreateCommandPool(apiCore.device, &poolInfo, nullptr, &apiCore.commandPools.graphicsPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create command pool");
	}

	// create transfer command pool
	poolInfo.queueFamilyIndex = apiCore.queues.transfer.index;
	if (vkCreateCommandPool(apiCore.device, &poolInfo, nullptr, &apiCore.commandPools.transferPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create command pool");
	}
}

void createSyncObjects() {
	// apiCore.sync.imagesInFlight.resize(apiCore.swapchain.images.size(), VK_NULL_HANDLE);

	VkSemaphoreCreateInfo semaphoreInfo = {};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo = {};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	if (vkCreateSemaphore(apiCore.device, &semaphoreInfo, nullptr, &apiCore.sync.imageAvailableSemaphore) != VK_SUCCESS ||
		vkCreateSemaphore(apiCore.device, &semaphoreInfo, nullptr, &apiCore.sync.renderFinishedSemaphore) != VK_SUCCESS ||
		vkCreateFence(apiCore.device, &fenceInfo, nullptr, &apiCore.sync.inFlightFence)) {
		
		throw std::runtime_error("failed to create semaphores");
	}
}

void allocateCommandBuffers() {
	apiCore.commandBuffers.data.resize(apiCore.swapchain.images.size() * 2);
	apiCore.commandBuffers.active = apiCore.commandBuffers.data.data();
	apiCore.commandBuffers.size = apiCore.swapchain.images.size();

	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = apiCore.commandPools.graphicsPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = apiCore.commandBuffers.data.size();

	if (vkAllocateCommandBuffers(apiCore.device, &allocInfo, apiCore.commandBuffers.data.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to create command buffers");
	}
}

void setClearColor(const Color& color) {
	apiCore.swapchain.clearColor.float32[0] = color.r;
	apiCore.swapchain.clearColor.float32[1] = color.g;
	apiCore.swapchain.clearColor.float32[2] = color.b;
	apiCore.swapchain.clearColor.float32[3] = color.a;
}

} // namespace age::core
