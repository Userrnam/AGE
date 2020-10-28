#include <chrono>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "Application.hpp"

#include "../Graphics/Rendering/Renderer.hpp"
#include "../Graphics/View/ViewManager.hpp"
#include "../Graphics/PositionManager.hpp"
#include "../Graphics/Core/Window/Window.hpp"
#include "../Graphics/Core/Core.hpp"
#include "../Graphics/Memory/DeviceAlloc.hpp"
#include "../Graphics/Core/CoreConfiguration.hpp"

#include "../Audio/Core.hpp"
#include "../Audio/Listener.hpp"

#include "../Scene/Scene.hpp"

#include "../Utils/utils.hpp"
#include "../Animation/Animator.hpp"
#include "../UI/UIManager.hpp"

namespace age {

extern FT_Library ftLibrary;
audio::Core audioCore;

PositionManager* defaultPositionManager;
PositionManager* selectedPositionManager;

auto currentTime = std::chrono::high_resolution_clock::now();

inline void initFreetype() {
    if (FT_Init_FreeType(&ftLibrary)) {
        throw std::runtime_error("initFreeType: failed to init");
    }
}

Application::Application(const std::string& name, int width, int height) {
    config::setApplicationName(name);
    config::setWindowProperties(
        config::WindowProperties()
        .setTitle(name)
        .setResizeEnable(false)
        .setSize(width, height)
    );

    create();
}

Application::~Application() {
    if (m_activeScene) {
        delete m_activeScene;
    }
    
    delete defaultPositionManager;

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
    Renderer::create();
    core::deviceAlloc::init();

    defaultPositionManager = new PositionManager;
    
    selectedPositionManager = defaultPositionManager;

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

    Listener::setDefaults();

    Sampler::createDefault();
    
    EventManager::init();
}

void Application::run() {
    onCreate();

    auto startTime = std::chrono::high_resolution_clock::now();
    auto previousTime = std::chrono::high_resolution_clock::now();

    int count = 0;
    float timeAvg = 0;

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
            this->onEvent(event);
            m_activeScene->handleEvent(event);
        }
        EventManager::clearEvents();
        Animator::update(elapsedTime);
        ViewManager::updateViews(elapsedTime, std::chrono::duration<float, std::chrono::seconds::period>(startTime - currentTime).count());

        m_activeScene->update(elapsedTime);

        // if (m_switchScene) {
        //     vkDeviceWaitIdle(core::apiCore.device);

        //     delete m_activeScene;

        //     m_activeScene = m_switchScene;
        //     m_switchScene = nullptr;

        //     continue;
        // }

        render();

        int fail = core::window::present();
        if (fail) {
            Renderer::rerender();
        }

        timeAvg += elapsedTime;

        if (count % 20 == 0) {
            m_fps = 20.0f / timeAvg;
            timeAvg = 0;
            count = 0;
        }

        count++;
    }

    vkDeviceWaitIdle(core::apiCore.device);
    onDestroy();
}

void Application::render() {
    auto& viewIds = m_activeScene->getViewIds();

    std::vector<entt::entity> ids;
    std::vector<age::Drawable> targets;
    targets.reserve(256);

    for (auto vId : viewIds) {
        auto& view = age::ViewManager::getView(vId);
        auto pm = view.getPositionManager();

        age::Positionable p;
        p.pos = view.getPosition() - view.getOrigin();
        p.size = { (float)age::core::apiCore.window.width, (float)age::core::apiCore.window.height };
        p.size /= view.getScale();

        auto ve = pm->getVisibleEntities(p);

        for (auto e : ve) {
            ids.push_back(e);
            targets.push_back(m_activeScene->m_registry.get<age::Drawable>(e));
        }
    }

    if (Renderer::renderRequired(ids)) {
        Renderer::render(targets);
    }
}

void Application::deleteActiveScene() {
    delete m_activeScene;
}

void Application::setWindowTitle(const std::string& s) {
    glfwSetWindowTitle(core::apiCore.window.handle, s.c_str());
}

} // namespace age
