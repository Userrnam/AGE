#pragma once

#include <glm/glm.hpp>

#include "Graphics/Renderer.hpp"

#include "View.hpp"
#include "Scene.hpp"
#include "EventManager.hpp"
#include "Event.hpp"

namespace age {

// how to make transition?
class Application {
    bool m_isRunning = true;

    void update();
protected:
    Scene* pActiveScene = nullptr;
    // not sure if we need this
    std::vector<View> m_views;


    void setActiveScene(Scene* scene) {
        pActiveScene = scene;
        scene->onCreate();
    }

    virtual void onEvent(Event event) {}
    virtual void onCoreConfig() {}
    virtual void onCreate() {}
    virtual void onDestroy() {}

public:
    Renderer m_renderer;
    Application() {}

    ~Application();
    void run();
    void stop() {
        m_isRunning = false;
    }
};

} // namespace age
