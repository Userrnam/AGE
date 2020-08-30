#pragma once

#include <glm/glm.hpp>
#include <unordered_map>

#include "Graphics/Renderer.hpp"
#include "Graphics/Sampler.hpp"

#include "EventManager.hpp"
#include "Event.hpp"
#include "Graphics/Font.hpp"

namespace age {

class Scene;
class Application {
    bool m_isRunning = true;
    bool m_created = false;
    std::unordered_map<uint64_t, Font> m_fonts;

    friend class Scene;
protected:

    Scene* pActiveScene = nullptr;

    void setActiveScene(Scene* scene) {
        pActiveScene = scene;
    }

    inline Font& getFont(const std::string& fontName) {
        return m_fonts[hash(fontName.c_str())];
    }

    inline void loadFont(const std::string& path, const std::string& fontName, unsigned fontSize = 100, Sampler sampler = Sampler::getDefault()) {
        Font font;
        font.load(path, fontSize, sampler);
        m_fonts[hash(fontName.c_str())] = font;
    }

    virtual void onEvent(Event event) {}
    virtual void onCoreConfig() {}
    virtual void onCreate() {}
    virtual void onDestroy() {}

public:
    void destroy();
    void create();
    void run();
    void stop() {
        m_isRunning = false;
    }
};

} // namespace age
