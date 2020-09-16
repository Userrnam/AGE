#pragma once

#include "Graphics.hpp"

#include "Utils/utils.hpp"
#include "m.hpp"

class RectController : public age::ScriptComponent {
    age::Transformable transformable;
    // TODO: these 2 can use same array
    // Fixme: bundle component does not work on linux
    age::BundleComponent<age::Transform> vars;
    age::StorageComponent<age::TexCoords> texCoords;
    age::ArrayComponent<age::Color, age::PER_VERTEX> colors;

    const float speed = age::sc(250.0f, m);

    virtual void onUpdate(float elapsedTime) override {
        transformable.move(
            (age::isKeyPressed(GLFW_KEY_RIGHT) - age::isKeyPressed(GLFW_KEY_LEFT)) * speed * elapsedTime,
            (age::isKeyPressed(GLFW_KEY_UP) - age::isKeyPressed(GLFW_KEY_DOWN)) * speed * elapsedTime
        );
        vars.get().get<age::Transform>().set(transformable.getTransform());
        vars.upload();
    }
public:

    RectController(Entity e) : age::ScriptComponent(e) {
        vars.create();
        vars.get().get<age::Transform>().set(transformable.setScale(age::sc(200, 200, m)).getTransform());
        vars.upload();

        texCoords.create();

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
            texCoords,
            age::TextureComponent(getTexture("mountains"))
        );
    }

    ~RectController() {
        vars.destroy();
        colors.destroy();
        texCoords.destroy();
        getComponent<age::Drawable>().destroy();
    }
};
