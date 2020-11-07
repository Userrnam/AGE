#pragma once

#include <GLFW/glfw3.h>

#include <AGE/Graphics.hpp>
#include <AGE/Scene.hpp>
#include <AGE/Audio.hpp>
#include <AGE/Utils.hpp>


class RectController : public age::ScriptComponent {
    age::Transformable transformable;
    age::BundleComponent<age::Transform, age::TexCoords> bundle;
    age::ArrayComponent<age::Color, age::PER_VERTEX> colors;

    age::Sound sound;
    age::SoundSource source;

    const float speed = 250.0f;

    virtual void onUpdate(float elapsedTime) override {
        auto dx = (age::isKeyPressed(GLFW_KEY_RIGHT) - age::isKeyPressed(GLFW_KEY_LEFT)) * speed * elapsedTime;
        auto dy = (age::isKeyPressed(GLFW_KEY_UP) - age::isKeyPressed(GLFW_KEY_DOWN)) * speed * elapsedTime;
        auto r = (age::isKeyPressed(GLFW_KEY_Q) - age::isKeyPressed(GLFW_KEY_E)) * elapsedTime;

        transformable.move(dx, dy);
        transformable.rotate(r);

        bundle.get<age::Transform>().set(transformable.getTransform());
        bundle.upload();

        auto pos = transformable.getPosition();

        source.setPosition(pos.x / 400, pos.y / 300, 1);
        source.setVelocity(dx, dy, 1);
    }
public:

    RectController(Entity e) : age::ScriptComponent(e) {
        transformable.create(e);
        transformable.setScale(200, 200);

        bundle.create();
        bundle.set<age::Transform>(transformable.getTransform());
        bundle.set(age::TexCoords());
        bundle.upload();

        colors.create(4);
        colors.add({{1, 0, 0, 1}});
        colors.add({{1, 0, 0, 1}});
        colors.add({{0, 0, 1, 1}});
        colors.add({{0, 0, 1, 1}});
        colors.upload();

        addComponent<age::Drawable>(
            age::RECTANGLE_SHAPE,
            bundle,
            colors,
            age::TextureComponent(getTexture("mountains"))
        );

        sound.create();
        sound.load(age::getAssetPath("test.wav"));

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

        bundle.destroy();
        colors.destroy();

        getComponent<age::Drawable>().destroy();
        getScene()->getPositionManager()->remove(getEntityId());
    }
};
