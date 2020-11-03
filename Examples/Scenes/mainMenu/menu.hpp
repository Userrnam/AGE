#pragma once

#include <AGE/Math.hpp>
#include <AGE/Scene.hpp>
#include "AGE/UI.hpp"
#include <AGE/Utils.hpp>

#include "Options.hpp"
#include "../Background.hpp"

struct Particle : public age::ParticleBase {
    age::Vector2f velocity;

    static constexpr age::Vector4f startColor { 1.0f, 0.0f, 0.0f, 1.0f };
    static constexpr age::Vector4f endColor = { 1.0f, 1.0f, 0.0f, 0.0f };
    static constexpr float maxlt = 3.0f;
    static constexpr float minlt = 1.5f;

    static float maxLifeTime() {
        return maxlt;
    }

    void spawn() {
        color = startColor;
        // set velocity
        velocity.x = age::frandom(-80.0f, 80.0f);
        velocity.y = age::frandom(10.0f, 150.0f);

        // set size
        UnmanagedTransformable::setScale(5, 5);

        // set life time
        lifeTime = age::frandom(minlt, maxlt);
    }

    void update(float elapsedTime) {
        UnmanagedTransformable::move(velocity * elapsedTime);
        color = age::lerp(startColor, endColor, livedTime / lifeTime);
    }
};

class MainMenu : public age::Scene {
    uint64_t uiblockId;

public:
    MainMenu(age::Application* app) : age::Scene(app) {
        auto button1 = createEntity<TestSceneButton>();
        createEntity<age::ParticleSystem<Particle>>(1000);
        createStaticEntity<Background>(age::Color(0, 0, 0, 1));

        uiblockId = addUIBlock(
            age::UIBlock(1)
            .addButton(std::get<1>(button1))
        );

        auto& block = getUIBlock(uiblockId);
        block.move(100, 100);
    }
};
