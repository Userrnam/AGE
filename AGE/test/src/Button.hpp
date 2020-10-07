#pragma once

#include <iostream>

#include "Animation.hpp"
#include "Graphics.hpp"
#include "UI.hpp"

class Button : public age::ScriptComponent, public age::IButton {
    age::Transformable transformable;
    age::TextComponent text;

    age::StorageComponent<age::Transform> transform;
    age::StorageComponent<age::Color> color;

    uint64_t animId = 0;

public:
    Button(Entity e) : age::ScriptComponent(e) {
        transform.create();
        transform.set(transformable.setScale(1, 1).setPosition(150, 150).getTransform());
        color.create();
        color.set(glm::vec4{0, 0.8, 0, 1});

        text.create(getFont("courier"));
        text.setText("Button");

        addComponent<age::Drawable>(age::RECTANGLE_SHAPE,
            transform,
            text,
            color
        );

        getComponent<age::Drawable>().setInstanceCount(6);

        age::UIManager::addBlock(
            age::UIBlock().addButton(this)
        );

        updatePoints(transformable.getPosition(), transformable.getScale() * text.getSize());

        auto av = getView<age::Drawable>();
        for (auto e : av) {

        }
    }

    ~Button() {
        color.destroy();
        transform.destroy();
        text.destroy();
        getComponent<age::Drawable>().destroy();
    }

    virtual void onEnter() override {
        age::Animator::stopAnimation(animId);

        animId = age::Animator::addAnimation(
            age::StateAnimation<age::Color, age::linearFunction>(&color.get(), &color.getBuffer())
            .setLooping(false)
            .addState(age::AnimationState(color.get(), 0))
            .addState(age::AnimationState(age::Color(0, 1, 0, 1), 0))
        );
    }

    virtual void onLeave() override {
        age::Animator::stopAnimation(animId);

        animId = age::Animator::addAnimation(
            age::StateAnimation<age::Color, age::linearFunction>(&color.get(), &color.getBuffer())
            .setLooping(false)
            .addState(age::AnimationState(color.get(), 0))
            .addState(age::AnimationState(age::Color(0, 0.8, 0, 1), 0))
        );
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
