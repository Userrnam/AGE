#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdexcept>

#include "Window.hpp"
#include "../Core.hpp"
#include "../CoreConfig.hpp"
#include "../coreAPI.hpp"
#include "../../Rendering/Framebuffers.hpp"
#include "../../Rendering/RenderPass.hpp"
#include "../../Rendering/Renderer.hpp"
#include "../../View/ViewManager.hpp"
#include "../../../Events/Event.hpp"

namespace age::core {

extern CoreConfig coreConfig;

namespace window {

glm::ivec2 getWindowSize() {
    glm::ivec2 size;
    glfwGetWindowSize(apiCore.window.handle, &size.x, &size.y);

    return size;
}

void windowSizeCallback(GLFWwindow* window, int width, int height) {
    glfwSetWindowSize(apiCore.window.handle, coreConfig.window.width, coreConfig.window.height);
}

void create() {
    if (apiCore.window.handle) {
        throw std::runtime_error("AGE supports only one window");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    glfwWindowHint(GLFW_RESIZABLE, coreConfig.window.resizable);

    int width = 0;
    int height = 0;

    auto monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    bool fullScreen = coreConfig.window.width == 0 || coreConfig.window.height == 0;
    if (fullScreen) {
        width = mode->width;
        height = mode->height;
    } else {
        width = coreConfig.window.width;
        height = coreConfig.window.height;
    }

    apiCore.window.handle = glfwCreateWindow(width, height, coreConfig.window.title.c_str(), nullptr, nullptr);
    apiCore.window.width = width;
    apiCore.window.height = height;

    if (fullScreen) {
        glfwSetWindowMonitor(apiCore.window.handle, monitor, 0, 0, width, height, mode->refreshRate);
    }

    if (!coreConfig.window.resizable) {
        glfwSetWindowSizeLimits(apiCore.window.handle, width, height, width, height);
    }

    if (glfwCreateWindowSurface(apiCore.instance, apiCore.window.handle, nullptr, &apiCore.window.surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create surface");
    }
}

void destroy() {
    glfwDestroyWindow(apiCore.window.handle);
}

void pollEvents() {
    glfwPollEvents();
}

bool closed() {
    return glfwWindowShouldClose(apiCore.window.handle);
}

void cleanupSwapchain() {
    vkDeviceWaitIdle(apiCore.device);

    if (apiCore.multisampling.sampleCount != VK_SAMPLE_COUNT_1_BIT) {
        apiCore.multisampling.image.destroy();
    }
    
    Framebuffers::destroy();

	// destroy swapChain Views
	for (auto imageView : apiCore.swapchain.imageViews) {
		vkDestroyImageView(apiCore.device, imageView, nullptr);
	}

    vkDestroySwapchainKHR(apiCore.device, apiCore.swapchain.swapchain, nullptr);
}

void recreateSwapchain() {
    int width, height;
    glfwGetWindowSize(apiCore.window.handle, &width, &height);

    while (width == 0 || height == 0) {
        glfwGetWindowSize(apiCore.window.handle, &width, &height);
        glfwWaitEvents();
    }

    apiCore.window.width = width;
    apiCore.window.height = height;

    cleanupSwapchain();

    createSwapchain();
    createMultisamplingResources();
    Framebuffers::create(RenderPass::get());
}

int present() {
    vkWaitForFences(apiCore.device, 1, &apiCore.sync.inFlightFence, VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(apiCore.device, apiCore.swapchain.swapchain,
        UINT64_MAX, apiCore.sync.imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        recreateSwapchain();
        return 1;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image");
    }

    VkSemaphore waitSemaphores[] = { apiCore.sync.imageAvailableSemaphore };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &apiCore.commandBuffers.active[imageIndex];
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &apiCore.sync.renderFinishedSemaphore;

    vkResetFences(apiCore.device, 1, &apiCore.sync.inFlightFence);

    if (vkQueueSubmit(apiCore.queues.graphics.queue, 1, &submitInfo,
        apiCore.sync.inFlightFence) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer");
    }

    VkSwapchainKHR swapChains[] = { apiCore.swapchain.swapchain };
    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &apiCore.sync.renderFinishedSemaphore;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr;

    result = vkQueuePresentKHR(apiCore.queues.present.queue, &presentInfo);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || apiCore.framebufferResized) {
        apiCore.framebufferResized = false;
        recreateSwapchain();
        return 1;
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image");
    }

    return 0;
}

} // namespace age::core
} // namespace window
