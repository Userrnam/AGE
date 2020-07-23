#include <chrono>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "Application.hpp"
#include "Viewport.hpp"

#include "Graphics/Core/coreAPI.hpp"
#include "Graphics/Core/Window.hpp"
#include "Graphics/Core/Core.hpp"
#include "Core/Command.hpp"
#include "Audio/Core.hpp"

namespace age {

extern FT_Library ftLibrary;
audio::Core audioCore;

std::chrono::steady_clock::time_point currentTime;

Application::~Application() {
    // destroy audio
    audioCore.destroy();

    m_renderer.destroy();
}

inline void initFreetype() {
    if (FT_Init_FreeType(&ftLibrary)) {
        throw std::runtime_error("initFreeType: failed to init");
    }
}

void Application::run() {
    onCoreConfig();

    m_renderer.init();

    // init freetype
    initFreetype();

    // init audio
    audioCore.init();

    EventManager::init();

    onCreate();

    auto startTime = std::chrono::high_resolution_clock::now();

    while (m_isRunning) {
        m_isRunning = !core::window::closed();
        core::window::pollEvents();

        // calculate elapsed time
        currentTime = std::chrono::high_resolution_clock::now();
        float elapsedTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
        startTime = currentTime;

        // handle events
        auto events = EventManager::getEvents();
        for (auto event : events) {
            this->onEvent(event);
            pActiveScene->onEvent(event);
        }
        EventManager::clearEvents();

        pActiveScene->update(elapsedTime);
        core::window::present();
    }

    vkDeviceWaitIdle(core::apiCore.device);
    audioCore.destroy();
    EventManager::destroy();

    onDestroy();
}

} // namespace age
