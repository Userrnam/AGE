#pragma once

#include <utility>

#include "External/entt.hpp"
#include "SceneAPI.hpp"

namespace age {

class Entity {
    entt::entity m_entityId;
    SceneAPI* m_scene;

    friend class Scene;
    friend class ScriptComponent;
public:
    inline FontComponent* getFont(const std::string& font) {
		return m_scene->getFont(font);
	}

	inline Texture& getTexture(const std::string& texture) {
		return m_scene->getTexture(texture);
	}

	inline void loadTexture(const std::string& path, const std::string& textureName, const Sampler& sampler = Sampler::getDefault()) {
		m_scene->loadTexture(path, textureName, sampler);
	}

	inline void freeTexture(const std::string& textureName) {
		m_scene->freeTexture(textureName);
	}

    // Warning: all entitiy's components must be destroyed before calling this
    void destroy() {
        m_scene->m_registry.destroy(m_entityId);
    }

    template<typename T>
    inline T& addComponentNoCreate() {
        T component;
        m_scene->m_registry.emplace<T>(m_entityId, component);
        return m_scene->m_registry.get<T>(m_entityId);
    }

    template<typename T, typename... Args>
    inline T& addComponent(Args... args) {
        m_scene->m_registry.emplace<T>(m_entityId, T());
        auto& component = m_scene->m_registry.get<T>(m_entityId);
        component.create(std::forward<Args>(args)...);
        return component;
    }

    template<typename T>
    inline T& addComponentNoCreate(T component) {
        m_scene->m_registry.emplace<T>(m_entityId, component);
        return m_scene->m_registry.get<T>(m_entityId);
    }

    template<typename T>
    inline T& getComponent() {
        return m_scene->m_registry.get<T>(m_entityId);
    }
};

} // namespace age
