#pragma once

#include <vector>

#include "../External/entt.hpp"
#include "../Graphics/View/View.hpp"
#include "../Events/Event.hpp"
#include "Entity.hpp"
#include "ScriptComponent.hpp"

#include "Application.hpp"
#include "SceneBase.hpp"

namespace age {

class Scene : public SceneBase {
	friend class Application;
	friend class Entity;
protected:
public:
	Scene(class Application* app);
	virtual ~Scene();

	inline Entity createEntity() { return Entity(this); }

	template<typename Script, typename... Args>
	inline Script* createEntity(Args... args) {
		Entity e = createEntity();
		auto p = new Script(e, args...);
        e.addComponentNoCreate<ScriptComponent*>(p);
		return p;
	}

    template<typename... Component, typename... Exclude>
    SceneView<entt::entity, entt::exclude_t<Exclude...>, Component...> getView(entt::exclude_t<Exclude...> e = {}) {
        return SceneView<entt::entity, entt::exclude_t<Exclude...>, Component...>(this, e);
    }
};

template<typename...>
class SceneView;

template<typename... Component, typename... Exclude>
class SceneView<entt::entity, entt::exclude_t<Exclude...>, Component...> {
	SceneBase* m_scene;
	entt::basic_view<entt::entity, entt::exclude_t<Exclude...>, Component...> m_view;

	class SceneViewIterator final {
		SceneBase* m_scene;
		typename entt::basic_view<entt::entity, entt::exclude_t<Exclude...>, Component...>::iterator m_enttIterator;

	public:
		inline SceneViewIterator(SceneBase* scene, typename entt::basic_view<entt::entity, entt::exclude_t<Exclude...>, Component...>::iterator it)
			: m_scene(scene), m_enttIterator(it) {}
	
		inline SceneViewIterator & operator++()  { m_enttIterator++; return *this; }
        inline SceneViewIterator operator++(int) { ++m_enttIterator; return *this; }

        inline SceneViewIterator & operator--()  noexcept { m_enttIterator--; return *this; }
        inline SceneViewIterator operator--(int) noexcept { --m_enttIterator; return *this; }

        inline bool operator==(const SceneViewIterator &other) const noexcept { return other.m_enttIterator == m_enttIterator; }
        inline bool operator!=(const SceneViewIterator &other) const noexcept { return other.m_enttIterator != m_enttIterator; }

        inline Entity operator->() const { return Entity(m_scene, *m_enttIterator); }
        inline Entity operator*()  const { return Entity(m_scene, *m_enttIterator); }
	};

public:
	SceneView(SceneBase* scene, entt::exclude_t<Exclude...> e = {})
		: m_scene(scene), m_view(scene->getRegistry()->template view<Component...>(e)) {
	}

	inline size_t size() const { return m_view.size(); }
	inline entt::basic_view<entt::entity, entt::exclude_t<Exclude...>, Component...>& getEnttView() {
		return m_view;
	}

	using iterator = SceneViewIterator;

	inline iterator begin() { return iterator(m_scene, m_view.begin()); }
	inline iterator end()   { return iterator(m_scene, m_view.end()); }
};

} // namespace age
