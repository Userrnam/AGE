#include <chrono>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "Application.hpp"
#include "Viewport.hpp"

#include "Graphics/Core/CoreCreator.hpp"
#include "Graphics/Core/coreAPI.hpp"
#include "Graphics/Core/Window.hpp"
#include "Graphics/Core/Core.hpp"
#include "Graphics/Core/Command.hpp"

#include "Audio/Core.hpp"

namespace age {

extern FT_Library ftLibrary;
audio::Core audioCore;

bool commandBuffersNeedUpdate = true;
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
    // destroy audio
    audioCore.destroy();

    // destroy graphics
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

    // init graphics
    core::initCore();

    // init freetype
    initFreetype();

    // init audio
    audioCore.init();

    onCreate();

    for (Layer* layer : m_layers) {
        layer->onCreate();
    }

    if (m_layers.size() == 0) {
        std::cout << "Application has no layers\n";
        return;
    }

    auto startTime = std::chrono::high_resolution_clock::now();

    while (!core::window::closed()) {
        if (commandBuffersNeedUpdate) {
            updateCommandBuffers();
            commandBuffersNeedUpdate = false;
        }

        core::window::pollEvents();
        currentTime = std::chrono::high_resolution_clock::now();
        float elapsedTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
        startTime = currentTime;

        for (auto layer : m_layers) {
            layer->onUpdate(elapsedTime);
            layer->camera.m_uniform.m_time[0] = std::chrono::duration<float, std::chrono::seconds::period>(currentTime.time_since_epoch()).count();
            layer->camera.upload();
        }

        core::window::present();
    }

    vkDeviceWaitIdle(core::apiCore.device);

    onDestroy();
}

} // namespace age
