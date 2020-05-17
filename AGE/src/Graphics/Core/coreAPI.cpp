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

void debugEnable(bool b) {
	apiCore.debug.enable = b;
}

void setCoreConfig(const CoreConfig& config) {
	coreConfig = config;
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
	// FIXME: typecast
	VkSampleCountFlagBits maxSamples = getMaxSampleCount();
	if (coreConfig.multisampling.sampleCount > (SampleCount)maxSamples) {
		std::cout << "warning: sample count reduced to " << maxSamples << std::endl;
		coreConfig.multisampling.sampleCount = (SampleCount)maxSamples;
	}
	apiCore.multisampling.sampleCount = (VkSampleCountFlagBits)coreConfig.multisampling.sampleCount;

	apiCore.depth.format = findDepthFormat();
}

// FIXME: add queue choice
// Default: 0 computeQueues, 1 graphicsQueue | presentQueue, 1 transferQueue
void createLogicalDevice() {
	std::vector<VkQueueFamilyProperties> queues = getQueueFamilyProperties(apiCore.physicalDevice);

	bool graphicsQueueFound = false;
	// bool computeQueueFound = false;
	bool transferQueueFound = false;

	// try to find queue that supports only transfer or compute
	uint32_t index = 0;
	for (auto& queue : queues) {
		if (!transferQueueFound && queue.queueFlags == VK_QUEUE_TRANSFER_BIT) {
			transferQueueFound = true;
			apiCore.queues.transfer.index = index;
		// } else if (coreConfig.queue.compute && !computeQueueFound && queue.queueFlags == VK_QUEUE_COMPUTE_BIT) {
		// 	computeQueueFound = true;
		// 	apiCore.queues.compute.index = index;
		}
		index++;
	}

	index = 0;
	for (auto &queue : queues) {
		if ( (transferQueueFound && apiCore.queues.transfer.index == index)
		//  || (computeQueueFound && apiCore.queues.compute.index == index) 
			) {
			index++;
			continue;
		}
		if (!graphicsQueueFound && (queue.queueFlags & VK_QUEUE_GRAPHICS_BIT)) {
			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(apiCore.physicalDevice, index, apiCore.window.surface, &presentSupport);
			if (presentSupport) {
				apiCore.queues.graphics.index = index;
				graphicsQueueFound = true;
			}
		// } else if (coreConfig.queue.compute && !computeQueueFound && (queue.queueFlags & VK_QUEUE_COMPUTE_BIT)) {
		// 	apiCore.queues.compute.index = index;
		// 	computeQueueFound = true;
		} else if (!transferQueueFound && (queue.queueFlags & VK_QUEUE_TRANSFER_BIT)) {
			apiCore.queues.transfer.index = index;
			transferQueueFound = true;
		}
		index++;
	}

	if (!graphicsQueueFound) {
		throw std::runtime_error("graphics queue not found");
	}
	
	std::vector<uint32_t> queueFamilies = {
		apiCore.queues.graphics.index,
		apiCore.queues.transfer.index
	};

	// if (computeQueueFound) {
	// 	queueFamilies.push_back(apiCore.queues.compute.index);
	// }

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

// FIXME
	vkGetDeviceQueue(apiCore.device, apiCore.queues.graphics.index, 0, &apiCore.queues.graphics.queue);
	vkGetDeviceQueue(apiCore.device, apiCore.queues.transfer.index, 0, &apiCore.queues.transfer.queue);
	// if (computeQueueFound) {
	// 	vkGetDeviceQueue(apiCore.device, apiCore.queues.compute.index, 0, &apiCore.queues.compute.queue);
	// }
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
	ImageCreateInfo info = {};
	info.format = findDepthFormat();
	info.extent = apiCore.swapchain.extent;
	info.mipLevel = 1;
	info.numberOfSamples = apiCore.multisampling.sampleCount;
	info.tiling = VK_IMAGE_TILING_OPTIMAL;
	info.imageUsage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
	info.memoryProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	info.aspectFlags = VK_IMAGE_ASPECT_DEPTH_BIT;

	apiCore.depth.image.create(info);
}

void createMultisamplingResources() {
	if (apiCore.multisampling.sampleCount == VK_SAMPLE_COUNT_1_BIT) {
		return;
	}

	ImageCreateInfo info = {};
	info.format = apiCore.swapchain.format;
	info.extent = apiCore.swapchain.extent;
	info.mipLevel = 1;
	info.numberOfSamples = apiCore.multisampling.sampleCount;
	info.tiling = VK_IMAGE_TILING_OPTIMAL;
	info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT;
	info.memoryProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	info.aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;

	apiCore.multisampling.image.create(info);
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

// void createCamera() {
// 	VkDeviceSize bufferSize = sizeof(glm::mat4);

// 	BufferCreateInfo bufferInfo;
// 	bufferInfo.size = bufferSize;
// 	bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
// 	bufferInfo.memoryProperties = VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
// 	apiCore.camera.buffer.create(bufferInfo);
// 	// apiCore.staticCamera.buffer.create(bufferInfo);

// 	glm::mat4 identity = glm::mat4(1.0f);
// 	glm::mat4 proj = glm::ortho(
// 		0.0f, static_cast<float>(apiCore.swapchain.extent.width),
// 		static_cast<float>(apiCore.swapchain.extent.height), 0.0f,
// 		0.0f, 1.0f
// 	);

// 	proj[2][2] = 2.0f;

// 	proj = proj;

// 	apiCore.camera.buffer.loadData(&proj, sizeof(proj));

// 	UniformBuffer ubo;
// 	ubo.buffer = apiCore.camera.buffer.getBuffer();
// 	ubo.size = sizeof(glm::mat4);

// 	DescriptorInfo descriptorInfo;
// 	descriptorInfo.ubos.push_back(ubo);
// 	descriptorInfo.ubosBinding = 0;

// 	apiCore.camera.descriptor = getDescriptor(descriptorInfo).sets.back();

// 	// descriptorInfo.ubos.pop_back();
// 	// ubo.buffer = apiCore.staticCamera.buffer.getBuffer();
// 	// apiCore.staticCamera.descriptor = getDescriptor(descriptorInfo).sets.back();
// }

void setClearColor(const Color& color) {
	apiCore.swapchain.clearColor.float32[0] = color.r;
	apiCore.swapchain.clearColor.float32[1] = color.g;
	apiCore.swapchain.clearColor.float32[2] = color.b;
	apiCore.swapchain.clearColor.float32[3] = color.a;
}

void destroy() {
	// vkDeviceWaitIdle(apiCore.device);

	// apiCore.camera.buffer.destroy();

	// vkFreeCommandBuffers(apiCore.device, apiCore.commandPools.graphicsPool,
	// 	apiCore.commandBuffers.data.size(), apiCore.commandBuffers.data.data());

	// vkDestroySemaphore(apiCore.device, apiCore.sync.renderFinishedSemaphore, nullptr);
	// vkDestroySemaphore(apiCore.device, apiCore.sync.imageAvailableSemaphore, nullptr);
	// vkDestroyFence(apiCore.device, apiCore.sync.inFlightFence, nullptr);

	// for (auto dl : apiCore.descriptor.layouts) {
	// 	vkDestroyDescriptorSetLayout(apiCore.device, dl.layout, nullptr);
	// }

	// for (auto dp : apiCore.descriptor.pools) {
	// 	vkDestroyDescriptorPool(apiCore.device, dp.pool, nullptr);
	// }

	// vkDestroyCommandPool(apiCore.device, apiCore.commandPools.graphicsPool, nullptr);
	// vkDestroyCommandPool(apiCore.device, apiCore.commandPools.transferPool, nullptr);

	// for (auto pipelineLayoutRef : apiCore.pipelineLayouts) {
	// 	vkDestroyPipelineLayout(apiCore.device, pipelineLayoutRef.pipelineLayout, nullptr);
	// }

	// for (auto renderPassRef : apiCore.renderPasses) {
	// 	renderPassRef.destroy();
	// }

	// apiCore.multisampling.image.destroy();
	// apiCore.depth.image.destroy();

	// // destroy swapChain Views
	// for (auto imageView : apiCore.swapchain.imageViews) {
	// 	vkDestroyImageView(apiCore.device, imageView, nullptr);
	// }
	// vkDestroySwapchainKHR(apiCore.device, apiCore.swapchain.swapchain, nullptr);
	// vkDestroyDevice(apiCore.device, nullptr);

	// if (apiCore.debug.enable) {
	// 	destroyDebugUtilsMessengerEXT(apiCore.instance, apiCore.debug.messenger, nullptr);
	// }

	// vkDestroySurfaceKHR(apiCore.instance, apiCore.window.surface, nullptr);
	// vkDestroyInstance(apiCore.instance, nullptr);
	// glfwDestroyWindow(apiCore.window.handle);
	// glfwTerminate();
}

} // namespace age::core
