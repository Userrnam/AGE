#pragma once

#include <AGE/Math/Vector.hpp>
#include <AGE/UI.hpp>
#include <AGE/Scene.hpp>
#include <AGE/Graphics.hpp>

#include "../test/testScene.hpp"
#include "../FlappyBird/FlappyBirdScene.hpp"

#include "../CustomButton.hpp"

age::Color __activeColor = age::Color(0.4, 0.4, 0.4, 1.0);
age::Color __color = age::Color(0.8, 0.8, 0.8, 1.0);

struct TestSceneButton : public CustomButton {
    TestSceneButton(age::Entity e) 
        : CustomButton(e, "test scene", __activeColor, __color) {}

    virtual void onPress(unsigned button) override {
        getScene()->getApplication()->selectScene<TestScene>();
    }
};

struct FlappyBirdButton : public CustomButton {
    FlappyBirdButton(age::Entity e)
        : CustomButton(e, "flappy bird", __activeColor, __color) {}
    
    virtual void onPress(unsigned button) override {
        getScene()->getApplication()->selectScene<FlappyBirdScene>();
    }
};
