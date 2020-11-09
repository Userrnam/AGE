#pragma once

#include <iostream>

#include <AGE/Animation.hpp>
#include <AGE/Graphics.hpp>
#include <AGE/Scene.hpp>
#include <AGE/UI.hpp>

#include "../CustomButton.hpp"

class Button : public CustomButton {
    uint64_t animId = 0;

public:
    Button(Entity e, const std::string& s)
        : CustomButton(e, s, age::Color(0, 0.2, 0, 1), age::Color(0, 1, 0, 1)) {}
    
    ~Button() {
        getScene()->stopAnimation(animId);
    }

    virtual void onEnter() override {
        getScene()->stopAnimation(animId);

        animId = getScene()->addAnimation(
            age::StateAnimation<age::Color, age::linearFunction>(&buffer.get<age::Color>(), &buffer.getBuffer())
            .setLooping(false)
            .addState(age::AnimationState(buffer.get<age::Color>(), 0.2))
            .addState(age::AnimationState(activeColor))
        );
    }

    virtual void onLeave() override {
        getScene()->stopAnimation(animId);

        animId = getScene()->addAnimation(
            age::StateAnimation<age::Color, age::linearFunction>(&buffer.get<age::Color>(), &buffer.getBuffer())
            .setLooping(false)
            .addState(age::AnimationState(buffer.template get<age::Color>(), 0.1))
            .addState(age::AnimationState(color))
        );
    }

    virtual void onPress(unsigned button) override {
        std::cout << "pressed " << buttonText << std::endl;
    }
};
