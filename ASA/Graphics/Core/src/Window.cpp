#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdexcept>

#include "Window.hpp"
#include "Core.hpp"


namespace core {

extern Core apiCore;
extern CoreConfig coreConfig;

namespace window {

void create(int width, int height, const char *title) {
    if (apiCore.window.handle) {
        throw std::runtime_error("ASA supports only one window");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    apiCore.window.handle = glfwCreateWindow(width, height, title, nullptr, nullptr);

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

void present() {
    vkWaitForFences(apiCore.device, 1, &apiCore.sync.inFlightFences[apiCore.sync.currentFrame], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(apiCore.device, apiCore.swapchain.swapchain, 
        UINT64_MAX, apiCore.sync.imageAvailableSemaphores[apiCore.sync.currentFrame], VK_NULL_HANDLE, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        recreateSwapchain();
        return;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image");
    }

    // check if previous frame is using this image
    if (apiCore.sync.imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
        vkWaitForFences(apiCore.device, 1, &apiCore.sync.imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
    }
    // mark the image as now being in use by this frame
    apiCore.sync.imagesInFlight[imageIndex] = apiCore.sync.inFlightFences[apiCore.sync.currentFrame];

    // updateUniformBuffer(imageIndex);

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    VkSemaphore waitSemaphores[] = { apiCore.sync.imageAvailableSemaphores[apiCore.sync.currentFrame] };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &apiCore.commandBuffers[imageIndex];

    VkSemaphore signalSemaphores[] = { apiCore.sync.renderFinishedSemaphores[apiCore.sync.currentFrame] };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    vkResetFences(apiCore.device, 1, &apiCore.sync.inFlightFences[apiCore.sync.currentFrame]);

    if (vkQueueSubmit(apiCore.queues.graphics.queue, 1, &submitInfo,
        apiCore.sync.inFlightFences[apiCore.sync.currentFrame]) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer");
    }

    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = { apiCore.swapchain.swapchain };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr;

    result = vkQueuePresentKHR(apiCore.queues.graphics.queue, &presentInfo);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || apiCore.framebufferResized) {
        apiCore.framebufferResized = false;
        recreateSwapchain();
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image");
    }

    apiCore.sync.currentFrame = (apiCore.sync.currentFrame + 1) % coreConfig.maxFramesInFlight;
}

} // namespace window
} // namespace core
