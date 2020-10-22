#include <AGE/Graphics.hpp>
#include <AGE/Scene.hpp>


class Background : public age::StaticScriptComponent {
    age::Transformable transformable;

public:
    Background(Entity e) : age::StaticScriptComponent(e) {
        transformable.create(e);
        transformable.setScale(1600, 1200);

        addComponent<age::Drawable>(age::RECTANGLE_SHAPE, 
            addComponent<age::StorageComponent<age::Transform>>(transformable.getTransform()),
            addComponent<age::StorageComponent<age::Color>>(age::Vector4f(1, 0, 0, 1))
        );
    }

    ~Background() {
        transformable.destroy();
        
        getComponent<age::Drawable>().destroy();
        getComponent<age::StorageComponent<age::Transform>>().destroy();
        getComponent<age::StorageComponent<age::Color>>().destroy();
    }
};
