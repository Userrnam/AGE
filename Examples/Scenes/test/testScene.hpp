#pragma once

#include <AGE/Scene.hpp>
#include <AGE/Utils.hpp>

#include <string>

#include "RectController.hpp"
#include "HelloText.hpp"
#include "Background.hpp"
#include "Triangle.hpp"
#include "Button.hpp"


class TestScene : public age::Scene {   
    uint64_t uiblockId; 

public:
    TestScene(age::Application* app) : age::Scene(app) {
        createUIBlock();
        createEntity<HelloText>();
        createEntity<RectController>();
        createStaticEntity<Triangle>();
        createStaticEntity<Background>();
    }

    virtual void onUpdate(float elapsedTime) override {
        std::string s = "fps: " + std::to_string(getApplication()->getFps());
        getApplication()->setWindowTitle(s);
    }

    void createUIBlock() {
        auto button1 = createEntity<Button>("button1", age::Vector2f(0, 0));
        auto button2 = createEntity<Button>("button2", age::Vector2f(0, 0));
        auto button3 = createEntity<Button>("button3", age::Vector2f(0, 0));

        uiblockId = addUIBlock(
            age::UIBlock(3)
            .addButton(std::get<1>(button3))
            .addButton(std::get<1>(button2))
            .addButton(std::get<1>(button1))
            .alignVertically(10)
        );

        auto& block = getUIBlock(uiblockId);
        block.move(100, 100);
    }

};
