#pragma once

#include <AGE/Graphics.hpp>
#include <AGE/Graphics/ObjectCreation/Components/BundleComponent.hpp>
#include <AGE/Graphics/ObjectCreation/Components/ColorComponent.hpp>
#include <AGE/Scene.hpp>


class Background : public age::StaticScriptComponent {
    age::Transformable transformable;
    age::BundleComponent<age::Transform, age::Color> bundle;

public:
    Background(Entity e, const age::Color& color) : age::StaticScriptComponent(e) {
        transformable.create(e);
        transformable.setScale(1600, 1200);

        bundle.create();
        bundle.get<age::Transform>().set(transformable.getTransform());
        bundle.get<age::Color>().set(color);
        bundle.upload();

        addComponent<age::Drawable>(age::RECTANGLE_SHAPE, 
            bundle
        );
    }

    ~Background() {
        transformable.destroy();
        bundle.destroy();
        getComponent<age::Drawable>().destroy();
        getScene()->getPositionManager()->remove(getEntityId());
    }
};
