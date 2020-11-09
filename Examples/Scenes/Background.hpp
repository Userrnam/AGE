#pragma once

#include <AGE/Graphics.hpp>
#include <AGE/Graphics/ObjectCreation/Components/ColorComponent.hpp>
#include <AGE/Scene.hpp>


class Background : public age::StaticScriptComponent {
    age::Transformable transformable;
    age::StorageComponent<age::Transform, age::Color> buffer;

public:
    Background(Entity e, const age::Color& color) : age::StaticScriptComponent(e) {
        transformable.create(e);
        transformable.setScale(1600, 1200);

        buffer.create();
        buffer.get<age::Transform>().set(transformable.getTransform());
        buffer.get<age::Color>().set(color);
        buffer.upload();

        addComponent<age::Drawable>(age::RECTANGLE_SHAPE, 
            buffer
        );
    }

    ~Background() {
        transformable.destroy();
        buffer.destroy();
        getComponent<age::Drawable>().destroy();
        getScene()->getPositionManager()->remove(getEntityId());
    }
};
