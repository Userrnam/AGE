#include <chrono>

#include "Application.hpp"

#include "../Graphics/Rendering/Renderer.hpp"
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
#include "../Utils/Logger.hpp"
#include "../Containers/Arena.hpp"

namespace age {

audio::Core audioCore;

auto currentTime = std::chrono::high_resolution_clock::now();

Application::Application(const std::string& name, int width, int height) {
    config::setApplicationName(name);
    config::setWindowProperties(
        config::WindowProperties()
        .setTitle(name)
        .setResizeEnable(true)
        .setSize(width, height)
    );

    create();
}

Application::~Application() {
    if (m_activeScene) {
        delete m_activeScene;
    }
    
    for (auto& font : m_fonts) {
        font.second->destroy();
        delete font.second;
    }

    for (auto& texture: m_textures) {
        texture.second.destroy();
    }

    Shape::destroyManager();

    Sampler::destroyDefault();

    audio::Core::destroy();

    Renderer::destroy();
    Arena::destroy();
}

// maybe pass coreConfig here
void Application::create() {
    Arena::init(8 * 1024);
    Renderer::create();
    core::deviceAlloc::init();

    Shape::createManager();

    audio::Core::init();

    Listener::setDefaults();

    Sampler::createDefault();
    
    EventManager::init();
}

void Application::run() {
    auto startTime = std::chrono::high_resolution_clock::now();
    auto previousTime = std::chrono::high_resolution_clock::now();

    int count = 0;
    float timeAvg = 0;

    if (!m_activeScene) {
        Logger::error("no scene selected");
        return;
    }

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

        float runTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
        onUpdate(elapsedTime);
        m_activeScene->update(elapsedTime, runTime);

        if (m_switchScene) {
            vkDeviceWaitIdle(core::apiCore.device);

            delete m_activeScene;

            m_activeScene = m_switchScene;
            m_switchScene = nullptr;

            startTime = std::chrono::high_resolution_clock::now();
            previousTime = std::chrono::high_resolution_clock::now();
        }

        EventManager::clearEvents();
        Arena::flush();

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
}

void Application::render() {
    std::vector<entt::entity> targetIds;
    std::vector<age::Drawable> targets;
    targets.reserve(256);

    const View* views[] = { &m_activeScene->getStaticView(), &m_activeScene->getDynamicView() };
    for (int i = 0; i < 2; ++i) {
        auto pm = m_activeScene->getPositionManager();
        auto& view = *views[i];

        // fixme: this will not work if view is rotated
        age::Positionable p;
        p.pos = view.getPosition() - view.getOrigin();
        p.size = { (float)age::core::apiCore.window.width, (float)age::core::apiCore.window.height };
        p.size /= view.getScale();

        auto ve = pm->getVisibleEntities(p);

        for (auto e : ve) {
            if (std::find(targetIds.begin(), targetIds.end(), e) == targetIds.end()) {
                targetIds.push_back(e);
                targets.push_back(m_activeScene->m_registry.get<age::Drawable>(e));
            }
        }
    }

    Renderer::render(targets);
}

void Application::setWindowTitle(const std::string& s) {
    glfwSetWindowTitle(core::apiCore.window.handle, s.c_str());
}

Vector2i Application::getWindowSize() {
    auto s = core::window::getWindowSize();
    return Vector2i(s.x, s.y);
}

} // namespace age
