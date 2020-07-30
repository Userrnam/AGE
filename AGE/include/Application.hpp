#pragma once

#include <glm/glm.hpp>

#include "Graphics/Renderer.hpp"
#include "Graphics/Sampler.hpp"

#include "View.hpp"
#include "Scene.hpp"
#include "EventManager.hpp"
#include "Event.hpp"

namespace age {

// how to make transition?
class Application {
    bool m_isRunning = true;
    bool m_created = false;
    Renderer m_renderer;

    Shared<Sampler> defaultSampler;
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
    inline Shared<Sampler> getDefaultSamplerCopy() {
        return defaultSampler.copy();
    }

    inline void render(std::vector<Drawable>& targets) {
        m_renderer.render(targets);
    }

    void destroy();
    void create();
    void run();
    void stop() {
        m_isRunning = false;
    }
};

} // namespace age
