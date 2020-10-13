#pragma once

#include "Graphics.hpp"

#include "Utils/utils.hpp"

class RectController : public age::ScriptComponent {
    age::Transformable transformable;
    // TODO: these 2 can use same array
    // Fixme: bundle component does not work on linux
    age::BundleComponent<age::Transform> vars;
    age::StorageComponent<age::TexCoords> texCoords;
    age::ArrayComponent<age::Color, age::PER_VERTEX> colors;

    age::Sound sound;
    age::SoundSource source;

    const float speed = 250.0f;

    virtual void onUpdate(float elapsedTime) override {
        auto dx = (age::isKeyPressed(GLFW_KEY_RIGHT) - age::isKeyPressed(GLFW_KEY_LEFT)) * speed * elapsedTime;
        auto dy = (age::isKeyPressed(GLFW_KEY_UP) - age::isKeyPressed(GLFW_KEY_DOWN)) * speed * elapsedTime;
        transformable.move(dx, dy);
        vars.get().get<age::Transform>().set(transformable.getTransform());
        vars.upload();

        auto pos = transformable.getPosition();
        source.setPosition(pos.x / 400, pos.y / 300, 1);
        source.setVelocity(dx, dy, 1);
    }
public:

    RectController(Entity e) : age::ScriptComponent(e) {
        transformable.create(e);
        transformable.setScale(200, 200);

        vars.create();
        vars.get().get<age::Transform>().set(transformable.getTransform());
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

        sound.create();
        sound.load(age::getResourcePath("test.wav"));

        source.create();
        source.setLooping(true);
        source.setSound(sound);
        source.play();
    }

    ~RectController() {
        source.stop();

        source.destroy();
        sound.destroy();

        transformable.destroy();
        vars.destroy();
        colors.destroy();
        texCoords.destroy();
        getComponent<age::Drawable>().destroy();
    }
};
