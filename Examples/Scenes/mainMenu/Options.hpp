#pragma once

#include <AGE/Math/Vector.hpp>
#include <AGE/UI.hpp>
#include <AGE/Scene.hpp>
#include <AGE/Graphics.hpp>

#include "../test/testScene.hpp"
#include "../CustomButton.hpp"

struct TestSceneButton : public CustomButton {
    TestSceneButton(Entity e) 
        : CustomButton(e, "test scene", age::Color(0.4, 0.4, 0.4, 1.0), age::Color(0.8, 0.8, 0.8, 1.0)) {}

    virtual void onPress(unsigned button) override {
        getScene()->getApplication()->selectScene<TestScene>();
    }
};
