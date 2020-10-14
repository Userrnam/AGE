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
    Button(Entity e, const std::string& s, age::Vector2f pos = {0,0}) : age::ScriptComponent(e) {
        transform.create();
        color.create();
        color.set(age::Vector4f{0, 0.2, 0, 1});

        text.create(getFont("courier"));
        text.setText(s);

        transformable.create(e, text.getSize());
        transformable.setPosition(pos);
        transform.set(transformable.getTransform());

        addComponent<age::Drawable>(age::RECTANGLE_SHAPE,
            transform,
            text,
            color
        );

        getComponent<age::Drawable>().setInstanceCount(s.size());

        updatePoints(transformable.getPosition(), transformable.getScale() * text.getSize());

        auto av = getView<age::Drawable>();
        for (auto e : av) {

        }
    }

    ~Button()  {
        color.destroy();
        transform.destroy();
        text.destroy();
        transformable.destroy();
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
            .addState(age::AnimationState(age::Color(0, 0.2, 0, 1), 0))
        );
    }

    virtual void onPress(unsigned button) override {
        std::cout << "pressed\n";
    }

    virtual void setSize(const age::Vector2f& size) override {
        updatePoints(getPosition(), size);
        transformable.setScale(size);
        transform.set(transformable.getTransform());
    }

    virtual void setPosition(const age::Vector2f& pos) override {
        updatePoints(pos, getSize());
        transformable.setPosition(pos);
        transform.set(transformable.getTransform());
    }
};
