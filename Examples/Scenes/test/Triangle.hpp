#pragma once

#include "TestTriangle.hpp"

#include <AGE/Graphics.hpp>
#include <AGE/Scene.hpp>

struct Triangle : public age::StaticScriptComponent {
    TestTriangle triangle;

    Triangle(Entity e) : age::StaticScriptComponent(e) {
        triangle.create();
        addComponentNoCreate<age::Drawable>(triangle);

        age::Positionable p;
        p.pos = {};
        p.size = { 800, 600 };

        getScene()->getPositionManager()->update(getEntityId(), p);
    }

    ~Triangle() {
        getScene()->getPositionManager()->remove(getEntityId());
        triangle.destroy();
    }
};
