#pragma once

#include "Graphics.hpp"

#include "Utils/utils.hpp"
#include "m.hpp"

class HelloText : public age::StaticScriptComponent {
    age::TextComponent text;
    age::Transformable transformable;
    age::BundleComponent<age::Transform> ct;

public:
    HelloText(Entity e) : age::StaticScriptComponent(e) {
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
    }

    ~HelloText() {
        text.destroy();
        ct.destroy();
        getComponent<age::Drawable>().destroy();
    }
};
