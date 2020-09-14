#include "Graphics.hpp"
#include "Scene.hpp"


struct Background : public age::StaticScriptComponent {
    Background(Entity e) : age::StaticScriptComponent(e) {
       addComponent<age::Drawable>(age::RECTANGLE_SHAPE, 
            addComponent<age::StorageComponent<age::Transform>>(age::Transformable().setScale(1600, 1200).getTransform()),
            addComponent<age::StorageComponent<age::Color>>(glm::vec4(1, 0, 0, 1))
        );
    }

    ~Background() {
        getComponent<age::Drawable>().destroy();
        getComponent<age::StorageComponent<age::Transform>>().destroy();
        getComponent<age::StorageComponent<age::Color>>().destroy();
    }
};
