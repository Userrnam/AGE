#pragma once

#include <AGE/Math/Vector.hpp>
#include <AGE/Scene.hpp>
#include "AGE/UI.hpp"

#include "Options.hpp"
#include "../Background.hpp"

class MainMenu : public age::Scene {
    uint64_t uiblockId;

public:
    MainMenu(age::Application* app) : age::Scene(app) {
        auto button1 = createEntity<TestSceneButton>();
        createStaticEntity<Background>(age::Color(0, 0, 0, 1));

        uiblockId = addUIBlock(
            age::UIBlock(1)
            .addButton(std::get<1>(button1))
        );

        auto& block = getUIBlock(uiblockId);
        block.move(100, 100);
    }
};
