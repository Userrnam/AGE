#include <chrono>

#include "Application.hpp"

#include "Graphics/Core/coreAPI.hpp"
#include "Graphics/Core/Window.hpp"
#include "Graphics/Core/Core.hpp"
#include "Graphics/Core/Command.hpp"
#include "Viewport.hpp"

namespace age {


void Application::updateCommandBuffers() {
    // update active commandBuffer
    if (core::apiCore.commandBuffers.active == core::apiCore.commandBuffers.data.data()) {
        core::apiCore.commandBuffers.active += core::apiCore.swapchain.images.size();
    } else {
        core::apiCore.commandBuffers.active -= core::apiCore.swapchain.images.size();
    }

    VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = 0;
	beginInfo.pInheritanceInfo = nullptr;

	for (size_t i = 0; i < core::apiCore.commandBuffers.size; ++i) {
        vkResetCommandBuffer(core::apiCore.commandBuffers.active[i], 0);
		if (vkBeginCommandBuffer(core::apiCore.commandBuffers.active[i], &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("failed to begin recording command buffer");
		}

		draw(i);

		if (vkEndCommandBuffer(core::apiCore.commandBuffers.active[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to record command buffer");
		}
	}
}

glm::vec2 Application::getWindowSize() {
    glm::vec2 out;
    out.x = core::apiCore.swapchain.extent.width;
    out.y = core::apiCore.swapchain.extent.height;
    return out;
}

void Application::draw(int i) {
    core::cmd::clear(i);
}

void Application::create() {
    onCreate();
    updateCommandBuffers();
}

void Application::run() {
    static auto startTime = std::chrono::high_resolution_clock::now();

    while (!core::window::closed()) {
        core::window::pollEvents();
        auto currentTime = std::chrono::high_resolution_clock::now();
        float elapsedTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
        startTime = currentTime;
        onUpdate(elapsedTime);
        core::window::present();
    }

    vkDeviceWaitIdle(core::apiCore.device);
    onDelete();
}

} // namespace age
