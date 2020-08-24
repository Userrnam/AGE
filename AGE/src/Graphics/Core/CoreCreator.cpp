#include "CoreCreator.hpp"
#include "coreAPI.hpp"
#include "Window.hpp"
#include "Core.hpp"
#include "VulkanDebug.hpp"
#include "RenderPassManager.hpp"
#include "DeviceAlloc.hpp"

namespace age::core {

extern CoreConfig coreConfig;

void initCore() {
    init();
    window::create();
    pickPhysicalDevice();
    createLogicalDevice();
    createSwapchain();
    createMultisamplingResources();
	if (core::coreConfig.multisampling.sampleCount != VK_SAMPLE_COUNT_1_BIT) {
		createRenderPass(RENDER_PASS_MULTISAMPLING_BIT);
	}
    createCommandPools();
    createSyncObjects();
    allocateCommandBuffers();
}

void destroyCore() {
	deviceAlloc::destroy();

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

	apiCore.renderPass.destroy();

	apiCore.multisampling.image.destroy();
	// apiCore.depth.image.destroy();

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
