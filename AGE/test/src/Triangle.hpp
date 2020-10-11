#include "Graphics.hpp"
#include "Scene.hpp"
#include "TestTriangle.hpp"


struct Triangle : public age::StaticScriptComponent {
    TestTriangle triangle;
    entt::entity entity;

    Triangle(Entity e) : age::StaticScriptComponent(e) {
        entity = e.getEntityId();

        triangle.create();
        addComponentNoCreate<age::Drawable>(triangle);

        age::Positionable p;
        p.pos = {};
        p.size = { 800, 600 };

        age::defaultPositionManager->update(e.getEntityId(), p);
    }

    ~Triangle() {
        age::defaultPositionManager->remove(entity);
        triangle.destroy();
    }
};
