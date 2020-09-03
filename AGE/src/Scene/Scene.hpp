#pragma once


#include <vector>

#include "External/entt.hpp"
#include "Graphics/View/View.hpp"
#include "Events/Event.hpp"
#include "Entity.hpp"
#include "ScriptComponent.hpp"

#include "Application.hpp"
#include "SceneAPI.hpp"

namespace age {

// class Application;
class Scene : public SceneAPI {
    void update(float elapsedTime);
	void handleEvent(Event event);

	friend class Application;
	friend class Entity;
protected:
	Entity createEntity() {
		Entity e;
		e.m_entityId = m_registry.create();
		e.m_scene = this;
		return e;
	}

	virtual void onDestroy() {}
	virtual void onCreate() {}

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
