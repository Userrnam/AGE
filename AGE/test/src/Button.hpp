#pragma once

#include <iostream>

#include "Graphics.hpp"
#include "UI.hpp"

class Button : public age::ScriptComponent, public age::IButton {
    age::Transformable transformable;

    age::StorageComponent<age::Transform> transform;
    age::StorageComponent<age::Color> color;

public:
    Button(Entity e) : age::ScriptComponent(e) {
        transform.create();
        transform.set(transformable.setScale(200, 50).setPosition(300, 300).getTransform());
        color.create();
        color.set(glm::vec4{0, 1, 0, 1});

        addComponent<age::Drawable>(age::RECTANGLE_SHAPE,
            transform,
            color
        );

        age::UIManager::addBlock(
            age::UIBlock().addButton(this)
        );

        updatePoints(transformable.getPosition() / 2.0f, transformable.getScale() / 2.0f);
    }

    ~Button() {
        color.destroy();
        transform.destroy();
        getComponent<age::Drawable>().destroy();
    }

    virtual void onEnter() override {
        color.set(glm::vec4(0, 0, 1, 1));
    }

    virtual void onLeave() override {
        color.set(glm::vec4(0, 1, 0, 1));
    }

    virtual void onPress(unsigned button) override {
    }

    virtual void setSize(const glm::vec2& size) override {
        transformable.setScale(size * 2.0f);
    }

    virtual void setPosition(const glm::vec2& pos) override {
        transformable.setPosition(pos * 2.0f);
    }
};
