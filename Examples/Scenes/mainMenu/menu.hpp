#pragma once

#include <AGE/Math.hpp>
#include <AGE/Scene.hpp>
#include <AGE/UI.hpp>
#include <AGE/Utils.hpp>

#include "Options.hpp"
#include "../Background.hpp"

struct Particle : public age::ParticleBase {
    age::Vector2f velocity;

    static constexpr age::Vector4f startColor { 1.0f, 0.0f, 0.0f, 1.0f };
    static constexpr age::Vector4f endColor = { 1.0f, 1.0f, 0.0f, 0.0f };
    static constexpr float maxlt = 1.0f;
    static constexpr float minlt = 0.5f;

    static float maxLifeTime() {
        return maxlt;
    }

    static age::Vector2f maxSize() {
        return age::Vector2f(400, 500);
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
    age::ParticleSystem<Particle>* ps;

public:
    MainMenu(age::Application* app) : age::Scene(app) {
        auto button1 = createEntity<TestSceneButton>();
        auto button2 = createEntity<FlappyBirdButton>();
        ps = createEntity<age::ParticleSystem<Particle>>(1000);

        ps->getTransformable().setOrigin(0.5f, 0.5f);

        createStaticEntity<Background>(age::Color(0, 0, 0, 1));

        uiblockId = addUIBlock(
            age::UIBlock(2)
            .addButton(button1)
            .addButton(button2)
            .alignVertically(10)
        );

        auto& block = getUIBlock(uiblockId);
        block.move(100, 100);
    }

    virtual void onEvent(const age::Event& e) override {
        if (e == age::event::KEY) {
            auto s = e.getStructure<age::event::Key>();
            if (s.key == GLFW_KEY_S) {
                ps->stop();
            } else if (s.key == GLFW_KEY_R) {
                ps->resume();
            }
        }
    }

    virtual void onUpdate(float elapsedTime) override {
        auto pos = age::getCursorPosition();
        auto& tr = ps->getTransformable();
        tr.setPosition(pos.x, pos.y);
        tr.informAboutUpdate();
    }
};
