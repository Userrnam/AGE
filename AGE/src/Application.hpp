#pragma once

#include <glm/glm.hpp>
#include <unordered_map>

#include "Graphics/Rendering/Renderer.hpp"
#include "Graphics/Objects/Sampler.hpp"

#include "Events/EventManager.hpp"
#include "Events/Event.hpp"
#include "Graphics/ObjectCreation/Components/FontComponent.hpp"

namespace age {

class Scene;
class Application {
    bool m_isRunning = true;
    bool m_created = false;
    float m_fps = 0;

    std::unordered_map<uint64_t, FontComponent*> m_fonts;
    std::unordered_map<uint64_t, Texture> m_textures;

    friend class Scene;
    friend class SceneBase;
protected:

    Scene* pActiveScene = nullptr;

    void setActiveScene(Scene* scene) {
        pActiveScene = scene;
    }

    inline FontComponent* getFont(const std::string& fontName) {
        return m_fonts[hash(fontName.c_str())];
    }

    inline void loadFont(const std::string& path, const std::string& fontName, unsigned fontSize = 50, Sampler sampler = Sampler::getDefault()) {
        auto font = new FontComponent;
        font->load(path, fontSize, sampler);
        m_fonts[hash(fontName.c_str())] = font;
    }

    inline Texture& getTexture(const std::string& textureName) {
        return m_textures[hash(textureName.c_str())];
    }

    inline void loadTexture(const std::string& path, const std::string& textureName, const Sampler& sampler = Sampler::getDefault()) {
        Texture texture;
        texture.create(path, sampler);
        m_textures[hash(textureName.c_str())] = texture;
    }

    inline void freeTexture(const std::string& textureName) {
        m_textures[hash(textureName.c_str())].destroy();
        m_textures.erase(hash(textureName.c_str()));
    }

    virtual void onEvent(Event event) {}
    virtual void onCoreConfig() {}
    virtual void onCreate() {}
    virtual void onDestroy() {}

public:

    float getFps() {
        return m_fps;
    }

    void setWindowTitle(const std::string& s);

    void destroy();
    void create();
    void run();
    void stop() {
        m_isRunning = false;
    }
};

} // namespace age
