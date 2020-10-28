#pragma once

#include <AGE/UI.hpp>
#include <AGE/Scene.hpp>
#include <AGE/Graphics.hpp>

#include "../test/testScene.hpp"

struct TestSceneButton : public age::ScriptComponent, public age::IButton {
    age::Transformable transformable;
    age::TextComponent text;
    std::string buttonText = "test scene";

    age::BundleComponent<age::Transform, age::Color> bundle;

    TestSceneButton(Entity e) : age::ScriptComponent(e) {
        text.create(getFont("courier"), buttonText.size());
        text.setText(buttonText);

        transformable.create(e, text.getSize());

        bundle.create();
        bundle.set<age::Color>(age::Vector4f(0.8f, 0.8f, 0.8f, 1.0f));
        bundle.set<age::Transform>(transformable.getTransform());
        bundle.upload();

        addComponent<age::Drawable>(age::RECTANGLE_SHAPE,
            bundle,
            text
        );

        getComponent<age::Drawable>().setInstanceCount(buttonText.size());

        updateHitBox({ transformable.getPosition().x, transformable.getPosition().y - text.getYShift() }, transformable.getScale() * text.getSize());
    }

    virtual void onPress(unsigned button) override {
        // getScene()->getApplication()->selectScene<TestScene>();
    }

    virtual void onEnter() override {
        bundle.set<age::Color>(age::Vector4f(0.9f, 0.9f, 0.9f, 1.0f));
        bundle.upload();
    }

    virtual void onLeave() override {
        bundle.set<age::Color>(age::Vector4f(0.1f, 0.8f, 0.8f, 1.0f));
        bundle.upload();
    }

    virtual void setSize(const age::Vector2f& size) override {
        transformable.setScale(size);
        bundle.set<age::Transform>(transformable.getTransform());
        bundle.upload();
    }

    virtual void move(const age::Vector2f& v) override {
        transformable.move(v);
        bundle.set<age::Transform>(transformable.getTransform());
        bundle.upload();
    }

    ~TestSceneButton() {
        bundle.destroy();
        text.destroy();
        transformable.destroy();
        getComponent<age::Drawable>().destroy();
    }
};
