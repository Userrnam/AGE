#pragma once

#include <GLFW/glfw3.h>

#include <iostream>

#include <AGE/Graphics.hpp>
#include <AGE/Animation.hpp>
#include <AGE/Scene.hpp>

class HelloText : public age::ScriptComponent {
    age::TextComponent text;
    age::Transformable transformable;
    age::BundleComponent<age::Transform> ct;
    age::TransformResolveStructure* pResolveStructure;
    uint64_t animId = 0;

public:
    HelloText(Entity e) : age::ScriptComponent(e) {
        text.create(getFont("courier"));
        text.setText("hello");

        transformable.create(e, text.getSize());
        transformable.move(25, 25);
        transformable.setScale(2, 1);
        transformable.rotate(0.3);

        ct.create();
        ct.get().get<age::Transform>().set(transformable.getTransform());
        ct.upload();

        addComponent<age::Drawable>(age::RECTANGLE_SHAPE,
            ct,
            text
        );

        getComponent<age::Drawable>().setInstanceCount(5);

        pResolveStructure = new age::TransformResolveStructure(&ct.get().get<age::Transform>(), &transformable);

        animId = getScene()->addAnimation(
            age::StateAnimation<age::Vector2f, age::linearFunction>(transformable.getPositionPointer(), &ct.getBuffer(), pResolveStructure)
            .setLooping(true)
            .addState(age::AnimationState(transformable.getPosition(), 1))
            .addState(age::AnimationState(age::Vector2f(100, 300), 2))
            .addState(age::AnimationState(age::Vector2f(300, 100), 2))
        );
    }

    ~HelloText() {
        delete pResolveStructure;

        text.destroy();
        transformable.destroy();
        ct.destroy();
        getComponent<age::Drawable>().destroy();
        getScene()->getPositionManager()->remove(getEntityId());
    }

    virtual void onEvent(age::Event e) override {
        if (e == age::event::KEY) {
            auto s = e.getStructure<age::event::Key>();
            if (s.action == GLFW_PRESS) {
                if (s.key == GLFW_KEY_T) {
                    getScene()->pauseAnimation(animId);
                } else if (s.key == GLFW_KEY_C) {
                    getScene()->continueAnimation(animId);
                }
            }
        }
    }
};
