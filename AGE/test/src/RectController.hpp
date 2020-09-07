#pragma once

#include "Graphics.hpp"

class RectController : public age::ScriptComponent {
    glm::vec2 move = {};
    age::Transformable transformable;
    // ComponentVariables variables;
    age::BundleComponent<age::Transform, age::TexCoords> vars;
    age::ArrayComponent<age::Color, age::PER_VERTEX> colors;
    const float speed = 1.0f;

    virtual void onCreate() override {
        transformable.setScale(100, 100);

        vars.create();
        vars.get().get<age::Transform>().set(transformable.setScale(500, 500).getTransform());
        vars.upload();

        colors.create(4);
        colors.add({{1, 1, 1, 1}});
        colors.add({{1, 1, 1, 1}});
        colors.add({{0, 0, 1, 1}});
        colors.add({{0, 0, 1, 1}});
        colors.upload();

        addComponent<age::Drawable>(
            age::RECTANGLE_SHAPE,
            vars,
            colors,
            age::TextureComponent(getTexture("mountains"))
        );
    }

    virtual void onEvent(age::Event event) override {
        if (event == age::event::KEY) {
            auto e = event.getStructure<age::event::Key>();
            if (e.action == GLFW_PRESS) {
                switch (e.key) {
                    case GLFW_KEY_UP: move.y = 10; break;
                    case GLFW_KEY_DOWN: move.y = -10; break;
                    case GLFW_KEY_RIGHT: move.x = 10; break;
                    case GLFW_KEY_LEFT: move.x = -10; break;
                    default:
                    break;
                }
            } else if (e.action == GLFW_RELEASE) {
                switch (e.key) {
                case GLFW_KEY_UP: case GLFW_KEY_DOWN: move.y = 0; break;
                case GLFW_KEY_LEFT: case GLFW_KEY_RIGHT: move.x = 0; break;
                
                default:
                    break;
                }
            }
        }
    }

    virtual void onUpdate(float elapsedTime) override {
        if (move != glm::vec2(0.0)) {
            transformable.move(move.x * speed, move.y * speed);
            vars.get().get<age::Transform>().set(transformable.getTransform());
            vars.upload();
        }
    }

    virtual void onDestroy() override {
        vars.destroy();
        colors.destroy();
        getComponent<age::Drawable>().destroy();
    }
};
