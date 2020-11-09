#pragma once

#include <AGE/Scene.hpp>
#include <AGE/Graphics.hpp>
#include <AGE/Events.hpp>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Column.hpp"


struct MyParticle : public age::ParticleBase {
    age::Vector2f velocity;

    static constexpr age::Vector4f startColor { 1.0f, 0.0f, 0.0f, 1.0f };
    static constexpr age::Vector4f endColor = { 1.0f, 1.0f, 0.0f, 0.0f };
    static constexpr float maxlt = 1.1f;
    static constexpr float minlt = 0.9f;

    static float maxLifeTime() {
        return maxlt;
    }

    static age::Vector2f maxSize() {
        return age::Vector2f(500, 500);
    }

    void spawn() {
        color = startColor;

        // set velocity
        velocity.x = age::frandom(-10.0f, 10.0f);
        velocity.y = age::frandom(-80.0f, -30.0f);

        // set size
        UnmanagedTransformable::setScale(5, 5);

        // set life time
        lifeTime = age::frandom(minlt, maxlt);
    }

    void update(float elapsedTime) {
        UnmanagedTransformable::move(velocity * elapsedTime);
        color = age::lerp(startColor, endColor, livedTime / lifeTime);
    }
};

struct Bird : public age::ScriptComponent {
    age::Transformable transformable;
    age::StorageComponent<age::Transform, age::Color, age::TexCoords> buffer;
    age::ParticleSystem<MyParticle>* ps;

    const float gravity = 50.0f;
    const float spacePressVelocity = 500.0f;
    float velocity = 0.0f;

    bool dead = false;

    Bird(age::Entity e) : age::ScriptComponent(e) {
        ps = getScene()->createEntity<age::ParticleSystem<MyParticle>>(1000);
        ps->stop();

        transformable.create(e);
        transformable.setScale(50, 50);
        transformable.setPosition(20, 300);

        buffer.create();
        buffer.set<age::Transform>(transformable.getTransform());
        buffer.set<age::Color>(age::Color(1, 1, 1, 1));
        buffer.set<age::TexCoords>(age::TexCoords());
        buffer.upload();

        age::setZ(100);
        addComponent<age::Drawable>(age::RECTANGLE_SHAPE,
            buffer,
            age::TextureComponent(getTexture("bird"))
        );
        age::setZ(0);
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
            ps->getTransformable().setPosition(transformable.getPosition() + transformable.getScale() / 2);
            ps->resume();
        } else {
            ps->stop();
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
                ps->stop();
            }
        }
    }
};
