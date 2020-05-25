#include <chrono>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "Application.hpp"
#include "Core/CoreCreator.hpp"

#include "Graphics/Core/coreAPI.hpp"
#include "Graphics/Core/Window.hpp"
#include "Graphics/Core/Core.hpp"
#include "Graphics/Core/Command.hpp"
#include "Viewport.hpp"

namespace age {

extern FT_Library ftLibrary;

std::chrono::steady_clock::time_point currentTime;

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

        for (auto& layer: m_layers) {
            layer->draw(i);
        }

		if (vkEndCommandBuffer(core::apiCore.commandBuffers.active[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to record command buffer");
		}
	}
}

Application::~Application() {
    for (Layer* layer : m_layers) {
        layer->destroy();
        delete layer;
    }
    core::destroyCore();
}

inline void initFreetype() {
    if (FT_Init_FreeType(&ftLibrary)) {
        throw std::runtime_error("initFreeType: failed to init");
    }
}

void Application::run() {
    onCoreConfig();

    core::initCore();
    initFreetype();

    onCreate();

    for (Layer* layer : m_layers) {
        layer->onCreate();
    }

    if (m_layers.size() == 0) {
        std::cout << "Application has no layers\n";
        return;
    }

    updateCommandBuffers();

    auto startTime = std::chrono::high_resolution_clock::now();

    while (!core::window::closed()) {
        core::window::pollEvents();
        currentTime = std::chrono::high_resolution_clock::now();
        float elapsedTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
        startTime = currentTime;

        for (auto layer : m_layers) {
            layer->onUpdate(elapsedTime);
        }

        core::window::present();
    }

    vkDeviceWaitIdle(core::apiCore.device);

    onDestroy();
}

} // namespace age
