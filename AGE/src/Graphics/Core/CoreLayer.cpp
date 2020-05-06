#include "CoreLayer.hpp"
#include "coreAPI.hpp"
#include "Window.hpp"
#include "Core.hpp"
#include "VulkanDebug.hpp"

namespace age::core {

CoreLayer::CoreLayer() {
    init();
    window::create();
    pickPhysicalDevice();
    createLogicalDevice();
    createSwapchain();
    createDepthResources();
    createMultisamplingResources();
    createCommandPools();
    createSyncObjects();
    allocateCommandBuffers();
    // createCamera();
}

CoreLayer::~CoreLayer() {
    // apiCore.camera.buffer.destroy();
	vkFreeCommandBuffers(apiCore.device, apiCore.commandPools.graphicsPool,
		apiCore.commandBuffers.data.size(), apiCore.commandBuffers.data.data());

	vkDestroySemaphore(apiCore.device, apiCore.sync.renderFinishedSemaphore, nullptr);
	vkDestroySemaphore(apiCore.device, apiCore.sync.imageAvailableSemaphore, nullptr);
	vkDestroyFence(apiCore.device, apiCore.sync.inFlightFence, nullptr);

	for (auto dl : apiCore.descriptor.layouts) {
		vkDestroyDescriptorSetLayout(apiCore.device, dl.layout, nullptr);
	}

	for (auto dp : apiCore.descriptor.pools) {
		vkDestroyDescriptorPool(apiCore.device, dp.pool, nullptr);
	}

	vkDestroyCommandPool(apiCore.device, apiCore.commandPools.graphicsPool, nullptr);
	vkDestroyCommandPool(apiCore.device, apiCore.commandPools.transferPool, nullptr);

	for (auto pipelineLayoutRef : apiCore.pipelineLayouts) {
		vkDestroyPipelineLayout(apiCore.device, pipelineLayoutRef.pipelineLayout, nullptr);
	}

	for (auto renderPassRef : apiCore.renderPasses) {
		renderPassRef.destroy();
	}

	apiCore.multisampling.image.destroy();
	apiCore.depth.image.destroy();

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
