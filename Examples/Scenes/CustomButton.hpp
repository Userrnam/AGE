#pragma once

#include <AGE/Graphics/ObjectCreation/Drawable.hpp>
#include <AGE/Math/Vector.hpp>
#include <AGE/UI.hpp>
#include <AGE/Scene.hpp>
#include <AGE/Graphics.hpp>


struct CustomButton : public age::ScriptComponent, public age::IButton {
    age::Transformable transformable;
    age::TextComponent text;
    age::Color color;
    age::Color activeColor;

    std::string buttonText = "test scene";

    age::BundleComponent<age::Transform, age::Color> bundle;

    CustomButton(Entity e, const std::string& title, age::Color _color, age::Color _activeColor) : age::ScriptComponent(e) {
        buttonText = title;
        color = _color;
        activeColor = _activeColor;

        text.create(getFont("courier"), buttonText.size());
        text.setText(buttonText);

        transformable.create(e, text.getSize());

        bundle.create();
        bundle.set<age::Color>(color);
        bundle.set<age::Transform>(transformable.getTransform());
        bundle.upload();

        addComponent<age::Drawable>(age::RECTANGLE_SHAPE,
            bundle,
            text
        );

        getComponent<age::Drawable>().setInstanceCount(buttonText.size());

        updateHitBox({ transformable.getPosition().x, transformable.getPosition().y - text.getYShift() }, transformable.getScale() * text.getSize());
    }

    ~CustomButton() {
        bundle.destroy();
        text.destroy();
        transformable.destroy();
        getComponent<age::Drawable>().destroy();
    }

    void setText(const std::string& s) {
        buttonText = s;
        text.setText(buttonText);
        getComponent<age::Drawable>().setInstanceCount(buttonText.size());
    }

    virtual void onEnter() override {
        bundle.set<age::Color>(activeColor);
        bundle.upload();
    }

    virtual void onLeave() override {
        bundle.set<age::Color>(color);
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
};
