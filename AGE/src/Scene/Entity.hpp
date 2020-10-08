#pragma once

#include <utility>

#include "External/entt.hpp"
#include "SceneAPI.hpp"

namespace age {

template<typename...>
class SceneView;

template<typename... Component, typename... Exclude>
class SceneView<entt::entity, entt::exclude_t<Exclude...>, Component...>;

class Entity {
    entt::entity m_entityId;
    SceneAPI* m_scene;

    friend class Scene;
    friend class StaticScriptComponent;
public:
    inline SceneAPI* getScene() { return m_scene; }

    Entity(SceneAPI* scene) {
        m_entityId = scene->m_registry.create();
        m_scene = scene;
    }

    Entity(SceneAPI* scene, entt::entity id) {
        m_scene = scene;
        m_entityId = id;
    }

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
    // ? possible memory leak
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

    entt::registry* getRegistry() const {
        return &m_scene->m_registry;
    }

    entt::entity getEntityId() const {
        return m_entityId;
    }

    template<typename... Component, typename... Exclude>
    SceneView<entt::entity, entt::exclude_t<Exclude...>, Component...> getView(entt::exclude_t<Exclude...> e = {}) {
        return SceneView<entt::entity, entt::exclude_t<Exclude...>, Component...>(m_scene, e);
    }
};

} // namespace age
