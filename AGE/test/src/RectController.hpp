#pragma once

#include "Graphics.hpp"

class RectController : public age::ScriptComponent {
    age::Transformable transformable;
    // TODO: these 2 can use same array
    age::BundleComponent<age::Transform, age::TexCoords> vars;
    age::ArrayComponent<age::Color, age::PER_VERTEX> colors;
    const float speed = 10.0f;

    virtual void onUpdate(float elapsedTime) override {
        transformable.move(
            (age::isKeyPressed(GLFW_KEY_RIGHT) - age::isKeyPressed(GLFW_KEY_LEFT)) * speed,
            (age::isKeyPressed(GLFW_KEY_UP) - age::isKeyPressed(GLFW_KEY_DOWN)) * speed
        );
        vars.get().get<age::Transform>().set(transformable.getTransform());
        vars.upload();
    }
public:

    RectController(Entity e) : age::ScriptComponent(e) {
        transformable.setScale(100, 100);

        vars.create();
        vars.get().get<age::Transform>().set(transformable.setScale(500, 500).getTransform());
        vars.upload();

        colors.create(4);
        colors.add({{1, 0, 0, 1}});
        colors.add({{1, 0, 0, 1}});
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

    ~RectController() {
        vars.destroy();
        colors.destroy();
        getComponent<age::Drawable>().destroy();
    }
};
