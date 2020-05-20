#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdexcept>

#include "Window.hpp"
#include "Core.hpp"
#include "CoreConfig.hpp"

namespace age::core {

extern CoreConfig coreConfig;

namespace window {

void windowSizeCallback(GLFWwindow* window, int width, int height) {
    glfwSetWindowSize(apiCore.window.handle, coreConfig.window.width, coreConfig.window.height);
}

void create() {
    if (apiCore.window.handle) {
        throw std::runtime_error("AGE supports only one window");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    if (!coreConfig.window.resizable) {
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    }

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

void pollEvents() {
    glfwPollEvents();
}

bool closed() {
    return glfwWindowShouldClose(apiCore.window.handle);
}

void recreateSwapchain() {

}

#include <chrono>

std::chrono::steady_clock::time_point prevTime;
std::chrono::steady_clock::time_point curTime;

#define TIME_LOG(name) \
    curTime = std::chrono::high_resolution_clock::now(); \
    std::cout << name << ": " \
    << std::chrono::duration<float, std::chrono::seconds::period>(curTime - prevTime).count() * 6000 << " %\n"; \
    prevTime = curTime;


// static uint32_t imageIndex;
// static VkSemaphore signalSemaphore;

void present() {
    TIME_LOG("render start")
    vkWaitForFences(apiCore.device, 1, &apiCore.sync.inFlightFence, VK_TRUE, UINT64_MAX); // 554
    TIME_LOG("wait for fances")

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(apiCore.device, apiCore.swapchain.swapchain, // 215
        UINT64_MAX, apiCore.sync.imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

    TIME_LOG("acqure next image")

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        recreateSwapchain();
        return;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image");
    }

    VkSemaphore waitSemaphores[] = { apiCore.sync.imageAvailableSemaphore };
    // VkSemaphore signalSemaphores[] = { apiCore.sync.renderFinishedSemaphore };
    // signalSemaphore = apiCore.sync.renderFinishedSemaphore;
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

    vkResetFences(apiCore.device, 1, &apiCore.sync.inFlightFence); // 65

    TIME_LOG("reset fences")

    if (vkQueueSubmit(apiCore.queues.graphics.queue, 1, &submitInfo, // 1359
        apiCore.sync.inFlightFence) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer");
    }

    TIME_LOG("Queue Submit")

    VkSwapchainKHR swapChains[] = { apiCore.swapchain.swapchain };
    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &apiCore.sync.renderFinishedSemaphore;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr;

    result = vkQueuePresentKHR(apiCore.queues.present.queue, &presentInfo); // 484
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || apiCore.framebufferResized) {
        apiCore.framebufferResized = false;
        recreateSwapchain();
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image");
    }
    TIME_LOG("present")

    std::cout << "\n\n\n";
}

} // namespace age::core
} // namespace window
