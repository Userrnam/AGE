#pragma once

#include <GLFW/glfw3.h>

#include <iostream>

#include <AGE/Graphics.hpp>
#include <AGE/Animation.hpp>
#include <AGE/Scene.hpp>

class HelloText : public age::ScriptComponent {
    EVENT_CALLBACK(HelloText, keyPressed);

    age::TextComponent text;
    age::Transformable transformable;
    age::StorageComponent<age::Transform> buffer;
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

        buffer.create();
        buffer.set(transformable.getTransform());
        buffer.upload();

        addComponent<age::Drawable>(age::RECTANGLE_SHAPE,
            buffer,
            text
        );

        getComponent<age::Drawable>().setInstanceCount(5);

        pResolveStructure = new age::TransformResolveStructure(&buffer.get(), &transformable);

        animId = getScene()->addAnimation(
            age::StateAnimation<age::Vector2f, age::linearFunction>(transformable.getPositionPointer(), &buffer.getBuffer(), pResolveStructure)
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
        buffer.destroy();
        getComponent<age::Drawable>().destroy();
        getScene()->getPositionManager()->remove(getEntityId());
    }

    void keyPressed(const age::event::Key& e) {
        if (e.action == GLFW_PRESS) {
            if (e.key == GLFW_KEY_T) {
                getScene()->pauseAnimation(animId);
            } else if (e.key == GLFW_KEY_C) {
                getScene()->continueAnimation(animId);
            }
        }
    }
};
