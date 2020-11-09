#include <GLFW/glfw3.h>

#include "Scenes/test/testScene.hpp"
#include "Scenes/mainMenu/menu.hpp"

#ifndef CMAKE_DEFINITION
#define RESOURCE_PATH ""
#endif

struct Application : public age::Application {
    Application(const std::string& name, int width, int height)
        : age::Application(name, width, height) {
        loadFont(age::getAssetPath("Courier.dfont"), "courier");
        loadTexture(age::getAssetPath("mountains.png"), "mountains");
        loadTexture(age::getAssetPath("bird.png"), "bird");

        selectScene<MainMenu>();
    }

    virtual void onUpdate(float elapsedTime) override {
        std::string s = "fps: " + std::to_string(getFps());
        setWindowTitle(s);
    }

    virtual void onEvent(age::Event e) override {
        if (e == age::event::KEY) {
            auto s = e.getStructure<age::event::Key>();
            if (s.action == GLFW_PRESS && s.key == GLFW_KEY_P) {
                stop();
            }
            if (s.action == GLFW_PRESS && s.key == GLFW_KEY_R && (age::isKeyPressed(GLFW_KEY_RIGHT_CONTROL) || age::isKeyPressed(GLFW_KEY_LEFT_CONTROL) )) {
                selectScene<MainMenu>();
            }
        }
    }
};

struct MyRectangle : public age::ScriptComponent {
    age::Transformable transformable;
    age::StorageComponent<age::Transform> buffer;

    MyRectangle(Entity e) : age::ScriptComponent(e) {
        transformable.create(e);
        transformable.setScale(100, 100);

        buffer.create(transformable.getTransform());
        
        addComponent<age::Drawable>(age::RECTANGLE_SHAPE,
            buffer
        );
    }
    
    ~MyRectangle() {
        transformable.destroy();
        buffer.destroy();
        getComponent<age::Drawable>().destroy();
    }
};

struct MyScene : public age::Scene {
    MyScene(age::Application* app) : age::Scene(app) {
        createEntity<MyRectangle>();
    }
};

struct MyApp : public age::Application {
    MyApp(const std::string& s, int width, int height) : age::Application(s, width, height) {
        selectScene<MyScene>();
    }
};

int main(int argc, char* argv[]) {
    age::setAssetsPath(RESOURCE_PATH);

    auto app = new Application("app", 800, 600);
    // auto app = new MyApp("app", 800, 600);

    app->run();

    delete app;

    return 0;
}
