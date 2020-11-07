#pragma once

#include <AGE/Scene.hpp>
#include <AGE/Graphics.hpp>
#include <AGE/Events.hpp>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Column.hpp"

struct Bird : public age::ScriptComponent {
    age::Transformable transformable;
    age::BundleComponent<age::Transform, age::Color, age::TexCoords> buffer;

    const float gravity = 50.0f;
    const float spacePressVelocity = 500.0f;
    float velocity = 0.0f;

    bool dead = false;

    Bird(age::Entity e) : age::ScriptComponent(e) {
        transformable.create(e);
        transformable.setScale(50, 50);
        transformable.setPosition(20, 300);

        buffer.create();
        buffer.set<age::Transform>(transformable.getTransform());
        buffer.set<age::Color>(age::Color(1, 1, 1, 1));
        buffer.set<age::TexCoords>(age::TexCoords());
        buffer.upload();

        addComponent<age::Drawable>(age::RECTANGLE_SHAPE,
            buffer,
            age::TextureComponent(getTexture("bird"))
        );
    }

    ~Bird() {
        transformable.destroy();
        buffer.destroy();

        getComponent<age::Drawable>().destroy();
    }

    virtual void onUpdate(float elapsedTime) override {
        if (dead) {
            velocity -= gravity;
            transformable.move(0, velocity * elapsedTime);
            buffer.set<age::Transform>(transformable.getTransform());
            buffer.upload();
            return;
        }

        if (age::isKeyPressed(GLFW_KEY_SPACE)) {
            velocity = spacePressVelocity;
        }

        velocity -= gravity;
        transformable.move(0, velocity * elapsedTime);
        buffer.set<age::Transform>(transformable.getTransform());
        buffer.upload();

        auto view = getView<ColumnTag>();

        // check for intersections
        age::Rectangle birdHitBox;
        birdHitBox.pos = transformable.getPosition();
        birdHitBox.size = transformable.getScale();

        for (auto column : view) {
            auto script = reinterpret_cast<Column*>(column.getComponent<StaticScriptComponent*>());

            if (birdHitBox.intersect(script->getHitBox())) {
                std::cout << "Bird died :(\n";
                buffer.set<age::Color>(age::Color(1, 0, 0, 1));
                dead = true;
            }
        }
    }
};
