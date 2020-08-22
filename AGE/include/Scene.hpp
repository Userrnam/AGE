#pragma once


#include <vector>

#include "entt.hpp"
#include "Graphics/View.hpp"
#include "Event.hpp"
#include "Entity.hpp"
#include "ScriptComponent.hpp"

namespace age {

class Application;
class Scene {
    void update(float elapsedTime);
	void handleEvent(Event event);

	friend class Application;
	friend class Entity;
protected:
	Application* parent = nullptr;
	std::vector<View> m_views;
	entt::registry m_registry;

	Entity createEntity() {
		Entity e;
		e.m_entityId = m_registry.create();
		e.m_registry = &m_registry;
		return e;
	}

	virtual void onDestroy() {}
	virtual void onCreate() {}

	// game logic goes here
	virtual void onEvent(Event event) {}
	virtual void onUpdate(float elapsedTime) {}
public:
	void create(Application* app);
	void destroy();

	// loads entities from file
	void load(const std::string& filename) {}
	// save entities to specified file
	void save(const std::string& filename) {}
};

} // namespace age
