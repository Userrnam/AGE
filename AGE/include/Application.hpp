#pragma once

#include <glm/glm.hpp>
#include <unordered_map>

#include "Graphics/Renderer.hpp"
#include "Graphics/Sampler.hpp"

#include "Scene.hpp"
#include "EventManager.hpp"
#include "Event.hpp"
#include "Graphics/Font.hpp"

namespace age {

class Application {
    bool m_isRunning = true;
    bool m_created = false;
    std::unordered_map<uint64_t, Font> m_fonts;
protected:
    Scene* pActiveScene = nullptr;

    void setActiveScene(Scene* scene) {
        pActiveScene = scene;
    }

    void loadFont(const std::string& path, const std::string fontName) {
        Font font;
    }

    virtual void onEvent(Event event) {}
    virtual void onCoreConfig() {}
    virtual void onCreate() {}
    virtual void onDestroy() {}

public:
    inline void render(const std::vector<RenderPack>& packs) {
        Renderer::render(packs);
    }

    void destroy();
    void create();
    void run();
    void stop() {
        m_isRunning = false;
    }
};

} // namespace age
