#include "Graphics.hpp"
#include "Scene.hpp"
#include "TestTriangle.hpp"


struct Triangle : public age::StaticScriptComponent {
    TestTriangle triangle;

    Triangle(Entity e) : age::StaticScriptComponent(e) {
        triangle.create();
        addComponentNoCreate<age::Drawable>(triangle);
    }

    ~Triangle() {
        triangle.destroy();
    }
};
