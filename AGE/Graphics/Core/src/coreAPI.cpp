#include <vector>
#include <string.h>
#include <iostream>
#include <set>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

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
	appInfo.pApplicationName = coreConfig.appInfo.name;
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
}

// FIXME: add queue choice
// Default: 0 computeQueues, 1 graphicsQueue | presentQueue, 1 transferQueue
void createLogicalDevice() {
	std::vector<VkQueueFamilyProperties> queues = getQueueFamilyProperties(apiCore.physicalDevice);

	bool graphicsQueueFound = false;
	bool computeQueueFound = false;
	bool transferQueueFound = false;

	// try to find queue that supports only transfer or compute
	uint32_t index = 0;
	for (auto& queue : queues) {
		if (!transferQueueFound && queue.queueFlags == VK_QUEUE_TRANSFER_BIT) {
			transferQueueFound = true;
			apiCore.queues.transfer.index = index;
		} else if (coreConfig.queue.compute && !computeQueueFound && queue.queueFlags == VK_QUEUE_COMPUTE_BIT) {
			computeQueueFound = true;
			apiCore.queues.compute.index = index;
		}
		index++;
	}

	index = 0;
	for (auto &queue : queues) {
		if ( (transferQueueFound && apiCore.queues.transfer.index == index) ||
			(computeQueueFound && apiCore.queues.compute.index == index) ) {
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
		} else if (coreConfig.queue.compute && !computeQueueFound && (queue.queueFlags & VK_QUEUE_COMPUTE_BIT)) {
			apiCore.queues.compute.index = index;
			computeQueueFound = true;
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

	if (computeQueueFound) {
		queueFamilies.push_back(apiCore.queues.compute.index);
	}

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
	if (computeQueueFound) {
		vkGetDeviceQueue(apiCore.device, apiCore.queues.compute.index, 0, &apiCore.queues.compute.queue);
	}
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

// FIXME: add user defined subpasses
void createRenderPass() {
	VkAttachmentDescription colorAttachment = {};
	colorAttachment.format = apiCore.swapchain.format;
	colorAttachment.samples = apiCore.multisampling.sampleCount;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

	if (apiCore.multisampling.sampleCount != VK_SAMPLE_COUNT_1_BIT) {
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	} else {
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	}

	VkAttachmentDescription depthAttachment = {};
	depthAttachment.format = findDepthFormat();
	depthAttachment.samples = apiCore.multisampling.sampleCount; // ??
	depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference colorAttachmentRef = {};
	colorAttachmentRef.attachment = 0;		// this is index
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depthAttachmentRef = {};
	depthAttachmentRef.attachment = 1;
	depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	
	VkSubpassDependency dependency = {};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	
	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;
	subpass.pDepthStencilAttachment = &depthAttachmentRef;
	
	VkAttachmentDescription colorAttachmentResolve = {};
	uint32_t attachmentCount = 2;
	if (apiCore.multisampling.sampleCount != VK_SAMPLE_COUNT_1_BIT) {
		colorAttachmentResolve.format = apiCore.swapchain.format;
		colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		
		VkAttachmentReference colorAttachmentResolveRef = {};
		colorAttachmentResolveRef.attachment = 2;
		colorAttachmentResolveRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		subpass.pResolveAttachments = &colorAttachmentResolveRef;
		attachmentCount += 1;
	}

	VkAttachmentDescription attachments[3] = { colorAttachment, depthAttachment, colorAttachmentResolve };
	VkRenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = attachmentCount;
	renderPassInfo.pAttachments = attachments;
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;

	if (vkCreateRenderPass(apiCore.device, &renderPassInfo, nullptr, &apiCore.renderPass) != VK_SUCCESS) {
		throw std::runtime_error("failed to create render pass");
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

	apiCore.depthImage.create(info);
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
	info.imageUsage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT;
	info.memoryProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	info.aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;

	apiCore.multisampling.image.create(info);
}

void createFramebuffers() {
	apiCore.swapchain.framebuffers.resize(apiCore.swapchain.imageViews.size());

	uint32_t attachmentCount = (apiCore.multisampling.sampleCount == VK_SAMPLE_COUNT_1_BIT) ? 2 : 3;
	VkImageView attachments[3] = {};
	size_t swapchainImageViewIndex = 0;
	// no multisampling
	if (attachmentCount == 2) {
		attachments[1] = apiCore.depthImage.getView();
	} else {
		attachments[0] = apiCore.multisampling.image.getView();
		attachments[1] = apiCore.depthImage.getView();
		swapchainImageViewIndex = 2;
	}

	for (size_t i = 0; i < apiCore.swapchain.imageViews.size(); ++i) {
		attachments[swapchainImageViewIndex] = apiCore.swapchain.imageViews[i];

		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = apiCore.renderPass;
		framebufferInfo.attachmentCount = attachmentCount;
		framebufferInfo.pAttachments = attachments;
		framebufferInfo.width = apiCore.swapchain.extent.width;
		framebufferInfo.height = apiCore.swapchain.extent.height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(apiCore.device, &framebufferInfo, nullptr, &apiCore.swapchain.framebuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create framebuffer");
		}
	}
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

// FIXME: this descriptor pool can allocate descriptors
// containing 1 or 0 ubos and 1 or 0 samplers
void createDescriptorPool(DescriptorUsage usage) {
	std::vector<VkDescriptorPoolSize> poolSizes;
	poolSizes.resize(2);
	
	uint32_t i = 0;
	if (usage & DescriptorUsage::UBO_BIT) {
		poolSizes[i].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSizes[i].descriptorCount = static_cast<uint32_t>(apiCore.swapchain.images.size());
		i++;
	}
	if (usage & DescriptorUsage::SAMPLER_BIT) {
		poolSizes[i].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		poolSizes[i].descriptorCount = static_cast<uint32_t>(apiCore.swapchain.images.size());
		i++;
	}

	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = i;
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = static_cast<uint32_t>(apiCore.swapchain.images.size());

	VkDescriptorPool dp;
	if (vkCreateDescriptorPool(apiCore.device, &poolInfo, nullptr, &dp) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor pool");
	}

	apiCore.descriptor.pools.push_back({dp, usage});
}

void createDescriptorSetLayout(uint32_t uboCount, uint32_t samplerCount) {
	uint32_t binding = 0;
	std::vector<VkDescriptorSetLayoutBinding> bindings;

	if (uboCount > 0) {
		VkDescriptorSetLayoutBinding uboLayoutBinding = {};
		uboLayoutBinding.binding = binding;
		uboLayoutBinding.descriptorCount = uboCount;
		uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		uboLayoutBinding.pImmutableSamplers = nullptr;
		bindings.push_back(uboLayoutBinding);
		binding++;
	}

	if (samplerCount > 0) {
		VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
		samplerLayoutBinding.binding = 1;
		samplerLayoutBinding.descriptorCount = samplerCount;
		samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		samplerLayoutBinding.pImmutableSamplers = nullptr;
		bindings.push_back(samplerLayoutBinding);
	}

	VkDescriptorSetLayoutCreateInfo layoutInfo = {};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
	layoutInfo.pBindings = bindings.data();

	VkDescriptorSetLayout descriptorSetLayout;
	if (vkCreateDescriptorSetLayout(apiCore.device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor set layout");
	}
	apiCore.descriptor.layouts.push_back({descriptorSetLayout, uboCount, samplerCount});
}

void createSyncObjects() {
	apiCore.sync.imageAvailableSemaphores.resize(coreConfig.maxFramesInFlight);
	apiCore.sync.renderFinishedSemaphores.resize(coreConfig.maxFramesInFlight);
	apiCore.sync.inFlightFences.resize(coreConfig.maxFramesInFlight);
	apiCore.sync.imagesInFlight.resize(apiCore.swapchain.images.size(), VK_NULL_HANDLE);

	VkSemaphoreCreateInfo semaphoreInfo = {};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo = {};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < coreConfig.maxFramesInFlight; ++i) {
		if (vkCreateSemaphore(apiCore.device, &semaphoreInfo, nullptr, &apiCore.sync.imageAvailableSemaphores[i]) != VK_SUCCESS ||
			vkCreateSemaphore(apiCore.device, &semaphoreInfo, nullptr, &apiCore.sync.renderFinishedSemaphores[i]) != VK_SUCCESS ||
			vkCreateFence(apiCore.device, &fenceInfo, nullptr, &apiCore.sync.inFlightFences[i])) {
			
			throw std::runtime_error("failed to create semaphores");
		}
	}
}

void allocateCommandBuffers() {
	apiCore.commandBuffers.data.resize(apiCore.swapchain.framebuffers.size() * 2);
	apiCore.commandBuffers.active = apiCore.commandBuffers.data.data();
	apiCore.commandBuffers.size = apiCore.swapchain.framebuffers.size();

	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = apiCore.commandPools.graphicsPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = apiCore.commandBuffers.data.size();

	if (vkAllocateCommandBuffers(apiCore.device, &allocInfo, apiCore.commandBuffers.data.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to create command buffers");
	}
}

void createCamera() {
	// create buffers
	VkDeviceSize bufferSize = sizeof(glm::mat4);

	apiCore.camera.buffers.resize(apiCore.swapchain.images.size());
	apiCore.camera.buffersMemory.resize(apiCore.swapchain.images.size());

	// for (size_t i = 0; i < swapChainImages.size(); ++i) {
	// 	createBuffer(
	// 		bufferSize,
	// 		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
	// 		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
	// 		uniformBuffers[i],
	// 		uniformBuffersMemory[i]
	// 	);
	// }

	createDescriptorPool(DescriptorUsage::UBO_BIT);
	createDescriptorSetLayout(1, 0);

	std::vector<VkDescriptorSetLayout> layouts(apiCore.swapchain.images.size(), apiCore.descriptor.layouts[0].layout);
	
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = apiCore.descriptor.pools[0].pool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(apiCore.swapchain.images.size());
	allocInfo.pSetLayouts = layouts.data();

	for (size_t i = 0; i < apiCore.swapchain.images.size(); ++i) {
		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = apiCore.camera.buffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(glm::mat4);
	}
}

void setClearColor(const Color& color) {
	apiCore.swapchain.clearColor.float32[0] = color.r;
	apiCore.swapchain.clearColor.float32[1] = color.g;
	apiCore.swapchain.clearColor.float32[2] = color.b;
	apiCore.swapchain.clearColor.float32[3] = color.a;
}

void destroy() {
	vkDeviceWaitIdle(apiCore.device);

	vkFreeCommandBuffers(apiCore.device, apiCore.commandPools.graphicsPool,
		apiCore.commandBuffers.data.size(), apiCore.commandBuffers.data.data());

	for (size_t i = 0; i < coreConfig.maxFramesInFlight; ++i) {
		vkDestroySemaphore(apiCore.device, apiCore.sync.renderFinishedSemaphores[i], nullptr);
		vkDestroySemaphore(apiCore.device, apiCore.sync.imageAvailableSemaphores[i], nullptr);
		vkDestroyFence(apiCore.device, apiCore.sync.inFlightFences[i], nullptr);
	}

	for (auto dl : apiCore.descriptor.layouts) {
		vkDestroyDescriptorSetLayout(apiCore.device, dl.layout, nullptr);
	}

	for (auto dp : apiCore.descriptor.pools) {
		vkDestroyDescriptorPool(apiCore.device, dp.pool, nullptr);
	}

	vkDestroyCommandPool(apiCore.device, apiCore.commandPools.graphicsPool, nullptr);
	vkDestroyCommandPool(apiCore.device, apiCore.commandPools.transferPool, nullptr);

	for (auto framebuffer : apiCore.swapchain.framebuffers) {
		vkDestroyFramebuffer(apiCore.device, framebuffer, nullptr);
	}

	apiCore.multisampling.image.destroy();
	apiCore.depthImage.destroy();
	// vkDestroyImage(apiCore.device, apiCore.multisampling.image, nullptr);
	// vkDestroyImageView(apiCore.device, apiCore.multisampling.imageView, nullptr);
	// vkFreeMemory(apiCore.device, apiCore.multisampling.imageMemory, nullptr);

	// vkDestroyImage(apiCore.device, apiCore.depth.image, nullptr);
	// vkDestroyImageView(apiCore.device, apiCore.depth.imageView, nullptr);
	// vkFreeMemory(apiCore.device, apiCore.depth.imageMemory, nullptr);

	vkDestroyRenderPass(apiCore.device, apiCore.renderPass, nullptr);

	// destroy swapChain Views
	for (auto imageView : apiCore.swapchain.imageViews) {
		vkDestroyImageView(apiCore.device, imageView, nullptr);
	}
	vkDestroySwapchainKHR(apiCore.device, apiCore.swapchain.swapchain, nullptr);
	vkDestroyDevice(apiCore.device, nullptr);

	if (apiCore.debug.enable) {
		destroyDebugUtilsMessengerEXT(apiCore.instance, apiCore.debug.messenger, nullptr);
	}

	vkDestroySurfaceKHR(apiCore.instance, apiCore.window.surface, nullptr);
	vkDestroyInstance(apiCore.instance, nullptr);

	glfwDestroyWindow(apiCore.window.handle);
	glfwTerminate();
}

} // namespace age::core
