#pragma once


#include <vector>

#include "entt.hpp"
#include "Graphics/View.hpp"
#include "Event.hpp"

namespace age {

/*
class Layer {
	entt::registry registry;
	VisibilityManager vm;

	// registry.insert<Component>(entity, ...);

public:
	// then use entt
	int createEntity();

	// uses entt
    void removeEntity(int eId);


	void update(float elapsedTime) {
		// update drawable
		// must be array (or vector)
		// should be our own data structure
		auto drawables = entt.view<Drawable>();
		auto visible = vm.getVisibleEntities();
		for (int vId : visible) {
			drawables[vId].draw();
		}

		// update other components
	}
};
*/

/*

entt::registry registry;
entity = registry.create();
registry.insert<Drawable>(entity, d)

all default drawable has pointer to Texture, color, position, transform
drawable has std::shared_ptr<Texture>

e = entt.registry.create();

registry.insert<Position>();
registry.insert<Size>();

// only data required for drawing (no position)
registry.insert<Drawable>();
// velocity, acceleration, friction, ...
registry.insert<Physics>();

thread physics
thread drawable
thread gamelogic

e_ids = vector<int>
e_ids.push_back(e);

for physics:
	f.update(dt)

for drawable:
	f.draw(dt);

get all entities in layer

// returns vector of ids
enemies = get_entities_in_layer(hash("enemy"))

position in physics, drawable, sound
registry.insert<Drawable>();

if (moved):
	registry.view<Sound>();
	... update positions

{
	d = sInfo.view<Drawable>();
	render(d)
*/

// how to handle events

class Application;

class Scene {
    void update(float elapsedTime);
	friend class Application;

protected:
	Application* parent = nullptr;
	std::vector<View> m_views;
	entt::registry registry;

	virtual void onDestroy() {}
	virtual void onCreate() {}

	// game logic goes here
	virtual void onEvent(Event event) {}
	virtual void onUpdate(float elapsedTime) {}
public:
	// loads entities from file
	void load(const std::string& filename) {}
	// save entities to specified file
	void save(const std::string& filename) {}

	void destroy() {
		onDestroy();
		for (auto& view : m_views) {
			view.destroy();
		}
		registry.clear();
	}

	Scene(Application* app) : parent(app) {}
};

} // namespace age
