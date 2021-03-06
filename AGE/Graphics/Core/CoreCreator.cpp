#include "CoreCreator.hpp"
#include "coreAPI.hpp"
#include "Window/Window.hpp"
#include "Core.hpp"
#include "VulkanDebug.hpp"
#include "../Memory/DeviceAlloc.hpp"
#include <GLFW/glfw3.h>

namespace age::core {

extern CoreConfig coreConfig;

void initCore() {
    init();
    window::create();
    pickPhysicalDevice();
    createLogicalDevice();
    createSwapchain();
    createMultisamplingResources();
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

	vkDestroyCommandPool(apiCore.device, apiCore.commandPools.graphicsPool, nullptr);
	vkDestroyCommandPool(apiCore.device, apiCore.commandPools.transferPool, nullptr);

	apiCore.multisampling.image.destroy();

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

	window::destroy();
	glfwTerminate();
}

} // namespace age::core
