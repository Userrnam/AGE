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
    Scene* m_activeScene = nullptr;
    Scene* m_switchScene = nullptr;

    std::unordered_map<uint64_t, FontComponent*> m_fonts;
    std::unordered_map<uint64_t, Texture> m_textures;

    friend class Scene;
    friend class SceneBase;

    void render();
    void create();

    void deleteActiveScene();

protected:
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
        auto it = m_textures.find(hash(textureName.c_str()));
        if (it == m_textures.end()) {
            throw std::runtime_error("freeTexture: texture hasn't been loaded: " + textureName);
        }
        it->second.destroy();
        m_textures.erase(it);
    }

    inline void freeFont(const std::string& fontName) {
        auto it = m_fonts.find(hash(fontName.c_str()));
        if (it == m_fonts.end()) {
            throw std::runtime_error("freeFont: font hasn't been loaded: " + fontName);
        }
        it->second->destroy();
        delete it->second;
        m_fonts.erase(it);
    }

public: 
    template<typename S, typename... Args>
    void selectScene(Args... args) {
        if (!m_activeScene) {
            m_activeScene = new S(this, args...);
        } else {
            m_switchScene = new S(this, args...);
        }
    }

    Application(const std::string& name, int width, int height);
    virtual ~Application();

    float getFps() {
        return m_fps;
    }

    void setWindowTitle(const std::string& s);
    Vector2i getWindowSize();

    void run();
    void stop() {
        m_isRunning = false;
    }
};

} // namespace age
