#pragma once

#include "Graphics.hpp"

#include "Utils/utils.hpp"

class HelloText : public age::ScriptComponent {
    age::TextComponent text;
    age::Transformable transformable;
    age::BundleComponent<age::Transform> ct;
    age::TransformResolveStructure* pResolveStructure;
    glm::vec2 pos;
    uint64_t animId = 0;

public:
    HelloText(Entity e) : age::ScriptComponent(e) {
        pos = { 25, 25 };
        text.create(getFont("courier"));
        text.setText("hello");
        ct.create();
        ct.get().get<age::Transform>().set(transformable.move(25, 25).setScale(2, 1).rotate(0.3).getTransform());
        ct.upload();

        addComponent<age::Drawable>(age::RECTANGLE_SHAPE,
            ct,
            text
        );

        getComponent<age::Drawable>().setInstanceCount(5);

        pResolveStructure = new age::TransformResolveStructure(&ct.get().get<age::Transform>(), &transformable);

        animId = age::Animator::addAnimation(
            age::StateAnimation<glm::vec2, age::linearFunction>(transformable.getPositionPointer(), &ct.getBuffer(), pResolveStructure)
            .setLooping(true)
            .addState(age::AnimationState(pos, 1))
            .addState(age::AnimationState(glm::vec2(100, 300), 2))
            .addState(age::AnimationState(glm::vec2(300, 100), 2))
        );
    }

    ~HelloText() {
        delete pResolveStructure;

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
};
