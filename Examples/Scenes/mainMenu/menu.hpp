#pragma once

#include <AGE/Scene.hpp>
#include "AGE/UI.hpp"

#include "Options.hpp"

class MainMenu : public age::Scene {
    uint64_t uiblockId;

public:
    MainMenu(age::Application* app) : age::Scene(app) {
        auto button1 = createEntity<TestSceneButton>();

        uiblockId = age::UIManager::addBlock(
            age::UIBlock(1)
            .addButton(std::get<1>(button1))
        );

        auto& block = age::UIManager::getBlock(uiblockId);
        block.move(100, 100);
    }

    ~MainMenu() {
        age::UIManager::eraseBlock(uiblockId);
    }
};
