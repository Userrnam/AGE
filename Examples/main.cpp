#include <GLFW/glfw3.h>

#include "Scenes/test/testScene.hpp"
#include "Scenes/mainMenu/menu.hpp"

#ifndef CMAKE_DEFINITION
#define RESOURCE_PATH ""
#endif

struct Application : public age::Application {
    EVENT_CALLBACK(Application, update);
    EVENT_CALLBACK(Application, keyPressed);

    Application(const std::string& name, int width, int height)
        : age::Application(name, width, height) {
        loadFont(age::getAssetPath("Courier.dfont"), "courier");
        loadTexture(age::getAssetPath("mountains.png"), "mountains");
        loadTexture(age::getAssetPath("bird.png"), "bird");

        selectScene<MainMenu>();
    }

    void update(const age::event::Update& e) {
        std::string s = "fps: " + std::to_string(getFps());
        setWindowTitle(s);
    }

    void keyPressed(const age::event::Key& s) {
        if (s.action == GLFW_PRESS && s.key == GLFW_KEY_P) {
            stop();
        }
        if (s.action == GLFW_PRESS && s.key == GLFW_KEY_R && (age::isKeyPressed(GLFW_KEY_RIGHT_CONTROL) || age::isKeyPressed(GLFW_KEY_LEFT_CONTROL) )) {
            selectScene<MainMenu>();
        }
    }
};

struct MyRectangle : public age::ScriptComponent {
    EVENT_CALLBACK(MyRectangle, update);

    age::Transformable transformable;
    age::StorageComponent<age::Transform> buffer;

    MyRectangle(Entity e) : age::ScriptComponent(e) {
        transformable.create(e);
        transformable.setScale(100, 100);

        buffer.create(transformable.getTransform());
        
        addComponent<age::Drawable>(age::RECTANGLE_SHAPE,
            staticView(),
            buffer
        );
    }
    
    ~MyRectangle() {
        transformable.destroy();
        buffer.destroy();
        getComponent<age::Drawable>().destroy();
    }

    void update(const age::event::Update& e) {
        float speed = 250.0f;
        auto dx = (age::isKeyPressed(GLFW_KEY_RIGHT) - age::isKeyPressed(GLFW_KEY_LEFT)) * speed * e.elapsedTime;
        auto dy = (age::isKeyPressed(GLFW_KEY_UP) - age::isKeyPressed(GLFW_KEY_DOWN)) * speed * e.elapsedTime;

        transformable.move(dx, dy);
        buffer.set(transformable.getTransform());
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
