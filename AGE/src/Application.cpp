#include "Events/Event.hpp"
#include "Graphics/Rendering/Renderer.hpp"
#include <chrono>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "Application.hpp"

#include "Graphics/Core/Window/Window.hpp"
#include "Graphics/Core/Core.hpp"
#include "Audio/Core.hpp"
#include "Scene/Scene.hpp"
#include "Graphics/View/ViewManager.hpp"

#include "Utils/utils.hpp"

namespace age {

extern FT_Library ftLibrary;
audio::Core audioCore;

auto currentTime = std::chrono::high_resolution_clock::now();

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
        font.second->destroy();
        delete font.second;
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
    ViewManager::add(view, hash("default"));
    ViewManager::select(hash("default"));

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
    auto previousTime = std::chrono::high_resolution_clock::now();

    while (m_isRunning) {
        m_isRunning = !core::window::closed();
        core::window::pollEvents();

        // calculate elapsed time
        currentTime = std::chrono::high_resolution_clock::now();
        float elapsedTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - previousTime).count();
        previousTime = currentTime;

        // handle events
        auto events = EventManager::getEvents();
        for (auto event : events) {
            if (event == age::event::RESIZE) {
                auto& view = ViewManager::getView(hash("default"));
                view.setViewport();
                // fixme:
                // Renderer::rerender();
            }
            this->onEvent(event);
            pActiveScene->handleEvent(event);
        }
        EventManager::clearEvents();
        ViewManager::updateViews(elapsedTime, std::chrono::duration<float, std::chrono::seconds::period>(startTime - currentTime).count());

        pActiveScene->update(elapsedTime);

        core::window::present();
    }
}

} // namespace age
