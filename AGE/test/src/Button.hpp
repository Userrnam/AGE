#pragma once

#include <iostream>

#include "Animation.hpp"
#include "Graphics.hpp"
#include "UI.hpp"

class Button : public age::ScriptComponent, public age::IButton {
    age::Transformable transformable;

    age::StorageComponent<age::Transform> transform;
    age::StorageComponent<age::Color> color;

public:
    Button(Entity e) : age::ScriptComponent(e) {
        transform.create();
        transform.set(transformable.setScale(100, 25).setPosition(150, 150).getTransform());
        color.create();
        color.set(glm::vec4{0, 1, 0, 1});

        addComponent<age::Drawable>(age::RECTANGLE_SHAPE,
            transform,
            color
        );

        age::UIManager::addBlock(
            age::UIBlock().addButton(this)
        );

        updatePoints(transformable.getPosition(), transformable.getScale());

        auto av = getView<age::Drawable>();
        for (auto e : av) {

        }
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
        std::cout << "pressed\n";
    }

    virtual void setSize(const glm::vec2& size) override {
        transformable.setScale(size * 2.0f);
    }

    virtual void setPosition(const glm::vec2& pos) override {
        transformable.setPosition(pos * 2.0f);
    }
};
