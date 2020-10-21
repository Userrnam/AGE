#pragma once

#include <unordered_map>
#include <string>

#include "../Events/EventManager.hpp"
#include "../Events/Event.hpp"
#include "../Graphics/ObjectCreation/Components/FontComponent.hpp"

namespace age {

class Scene;
class Application {
    bool m_isRunning = true;
    float m_fps = 0;

    std::unordered_map<uint64_t, FontComponent*> m_fonts;
    std::unordered_map<uint64_t, Texture> m_textures;

    friend class Scene;
    friend class SceneBase;

    void render();
    void create();

protected:
    Scene* pActiveScene = nullptr;

    void setActiveScene(Scene* scene) {
        pActiveScene = scene;
    }

    inline FontComponent* getFont(const std::string& fontName) {
        return m_fonts[hash(fontName.c_str())];
    }

    inline void loadFont(const std::string& path, const std::string& fontName, const FontInfo& info = FontInfo()) {
        auto font = new FontComponent;
        font->load(path, info);
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
    virtual void onCreate() {}
    virtual void onDestroy() {}

public:
    Application(const std::string& name, int width, int height);
    ~Application();

    float getFps() {
        return m_fps;
    }

    void setWindowTitle(const std::string& s);

    void run();
    void stop() {
        m_isRunning = false;
    }
};

} // namespace age
