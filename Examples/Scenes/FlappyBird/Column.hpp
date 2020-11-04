#pragma once

#include <AGE/Scene.hpp>
#include <AGE/Math.hpp>

struct ColumnTag {
    int dummy;

    void create() {}
};

struct Column : public age::StaticScriptComponent {
    age::Transformable transformable;
    age::StorageComponent<age::Transform> transformBuffer;

    const float columnHeight = 500.0f;

    Column(age::Entity e) : age::StaticScriptComponent(e) {
        transformable.create(e);
        transformable.setScale(50, columnHeight);

        transformBuffer.create(transformable.getTransform());

        addComponent<age::Drawable>(age::RECTANGLE_SHAPE,
            transformBuffer
        );

        addComponent<ColumnTag>();
    }

    ~Column() {
        transformable.destroy();
        transformBuffer.destroy();

        getComponent<age::Drawable>().destroy();
    }

    void setTop(age::Vector2f pos) {
        transformable.setPosition(pos.x, pos.y - columnHeight);
        transformBuffer.set(transformable.getTransform());
    }

    void setBottom(age::Vector2f pos) {
        transformable.setPosition(pos);
        transformBuffer.set(transformable.getTransform());
    }

    void move(float x) {
        transformable.move(x, 0);
        transformBuffer.set(transformable.getTransform());
    }

    bool isVisible() {
        if (transformable.getPosition().x < -70) {
            return false;
        }
        return true;
    }

    age::Rectangle getHitBox() {
        age::Rectangle res;
        res.pos = transformable.getPosition();
        res.size = transformable.getScale();
        return res;
    }
};
