#include <chrono>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "Application.hpp"
// #include "View/Viewport.hpp"

// #include "Graphics/Core/coreAPI.hpp"
#include "Graphics/Core/Window/Window.hpp"
#include "Graphics/Core/Core.hpp"
// #include "Core/Command.hpp"
#include "Audio/Core.hpp"
// #include "ShapeManager.hpp"
#include "Scene/Scene.hpp"
#include "Graphics/View/ViewManager.hpp"

namespace age {

extern FT_Library ftLibrary;
audio::Core audioCore;

std::chrono::steady_clock::time_point currentTime;

inline void initFreetype() {
    if (FT_Init_FreeType(&ftLibrary)) {
        throw std::runtime_error("initFreeType: failed to init");
    }
}

void Application::destroy() {
    vkDeviceWaitIdle(core::apiCore.device);

    pActiveScene->destroy();
    onDestroy();

    for (auto& font : m_fonts) {
        font.second.destroy();
    }

    for (auto& texture: m_textures) {
        texture.second.destroy();
    }

    ViewManager::destroy();
    Shape::destroyManager();

    Sampler::destroyDefault();

    audioCore.destroy();

    EventManager::destroy();
    Renderer::destroy();
}

// maybe pass coreConfig here
void Application::create() {
    m_created = true;

    onCoreConfig();

    Renderer::create();
    core::deviceAlloc::init();

    // create default view
    View view;
    view.create();
    ViewManager::add(view, "default");
    ViewManager::select("default");

    Shape::createManager();

    // init freetype
    initFreetype();

    // init audio
    audioCore.init();

    Sampler::createDefault();
    
    EventManager::init();

    onCreate();
}

void Application::run() {
    if (!m_created) {
        throw std::runtime_error("Application has not been created");
    }

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
            pActiveScene->handleEvent(event);
        }
        EventManager::clearEvents();
        ViewManager::updateViews(elapsedTime, std::chrono::duration<float, std::chrono::seconds::period>(currentTime.time_since_epoch()).count());

        pActiveScene->update(elapsedTime);

        core::window::present();
    }
}

} // namespace age
