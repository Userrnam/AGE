#pragma once

#include "Graphics.hpp"

#include "Utils/utils.hpp"
#include "m.hpp"

class HelloText : public age::ScriptComponent {
    age::TextComponent text;
    age::Transformable transformable;
    age::BundleComponent<age::Transform> ct;
    glm::vec2 pos;
    uint64_t animId = 0;

public:
    HelloText(Entity e) : age::ScriptComponent(e) {
        pos = age::sc(50, 50, m);
        text.create(getFont("courier"));
        text.setText("hello");
        ct.create();
        ct.get().get<age::Transform>().set(transformable.move(age::sc(50, 50, m)).setScale(2, 1).rotate(0.3).getTransform());
        ct.upload();

        addComponent<age::Drawable>(age::RECTANGLE_SHAPE,
            ct,
            text
        );

        getComponent<age::Drawable>().setInstanceCount(5);

        animId = age::Animator::addAnimation(
            age::StateAnimation<glm::vec2, age::linearFunction>(&pos)
            .setLooping(true)
            .addState(age::AnimationState(pos, 1))
            .addState(age::AnimationState(glm::vec2(200, 600), 2))
            .addState(age::AnimationState(glm::vec2(600, 200), 2))
        );
    }

    ~HelloText() {
        text.destroy();
        ct.destroy();
        getComponent<age::Drawable>().destroy();
    }

    virtual void onEvent(age::Event e) override {
        if (e == age::event::KEY) {
            auto s = e.getStructure<age::event::Key>();
            if (s.action == GLFW_PRESS) {
                if (s.key == GLFW_KEY_T) {
                    age::Animator::pauseAnimation(animId);
                } else if (s.key == GLFW_KEY_C) {
                    age::Animator::continueAnimation(animId);
                }
            }
        }
    }

    virtual void onUpdate(float elapsedTime) override {
        ct.get().get<age::Transform>().set(transformable.setPosition(pos).getTransform());
        ct.upload();
    }
};
