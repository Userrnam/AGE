#pragma once

#include "../External/entt.hpp"
#include "SceneBase.hpp"

namespace age {

template<typename...>
class SceneView;

template<typename... Component, typename... Exclude>
class SceneView<entt::entity, entt::exclude_t<Exclude...>, Component...>;

class Scene;
class Entity {
    entt::entity m_entityId;
    SceneBase* m_scene;

public:
    inline Scene* getScene() { return reinterpret_cast<Scene*>(m_scene); }

    Entity() {}
    Entity(SceneBase* scene) {
        m_entityId = scene->m_registry.create();
        m_scene = scene;
    }

    Entity(SceneBase* scene, entt::entity id) {
        m_scene = scene;
        m_entityId = id;
    }

    inline View& staticView(float z = 0.0f) { return m_scene->staticView(z); }
    inline View& dynamicView(float z = 0.0f) { return m_scene->dynamicView(z); }

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

    template<typename T>
    inline T& addComponentNoCreate() {
        T component;
        m_scene->m_registry.emplace<T>(m_entityId, component);
        return m_scene->m_registry.get<T>(m_entityId);
    }

    template<typename T, typename... Args>
    inline T& addComponent(const Args&... args) {
        m_scene->m_registry.emplace<T>(m_entityId, T());
        auto& component = m_scene->m_registry.get<T>(m_entityId);
        component.create(args...);
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
