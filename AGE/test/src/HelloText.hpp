#pragma once

#include "Graphics.hpp"

class HelloText : public age::ScriptComponent {
    age::TextComponent text;
    age::Transformable transformable;
    age::BundleComponent<age::Color, age::Transform> ct;

    virtual void onCreate() override {
        text.create(getFont("courier"));
        text.setText("hello");
        ct.create();
        ct.get().get<age::Transform>().set(transformable.move(100, 100).setScale(3, 1).getTransform());
        ct.upload();

        addComponent<age::Drawable>(age::RECTANGLE_SHAPE,
            ct,
            text
        );

        getComponent<age::Drawable>().setInstanceCount(5);
    }

    virtual void onDestroy() override {
        text.destroy();
        ct.destroy();
    }
};
