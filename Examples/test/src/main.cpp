#include <AGE/Scene.hpp>
#include <AGE/Utils.hpp>

#include <string>

#ifndef CMAKE_DEFINITION
#define RESOURCE_PATH ""
#endif

#include "RectController.hpp"
#include "HelloText.hpp"
#include "Background.hpp"
#include "Triangle.hpp"
#include "Button.hpp"


class TestScene : public age::Scene {    
    virtual void onUpdate(float elapsedTime) override {
        std::string s = "fps: " + std::to_string(getApplication()->getFps());
        getApplication()->setWindowTitle(s);
    }

    void createUIBlock() {
        auto button1 = createEntity<Button>("button1", age::Vector2f(0, 0));
        auto button2 = createEntity<Button>("button2", age::Vector2f(0, 0));
        auto button3 = createEntity<Button>("button3", age::Vector2f(0, 0));

        auto id = age::UIManager::addBlock(
            age::UIBlock(3)
            .addButton(std::get<1>(button2))
            .addButton(std::get<1>(button1))
            .addButton(std::get<1>(button3))
            .alignVertically(10)
        );

        auto& block = age::UIManager::getBlock(id);
        block.move(100, 100);
    }

    virtual void onCreate() override {
        createEntity<HelloText>();
        createUIBlock();
        createEntity<RectController>();
        createStaticEntity<Triangle>();
        createStaticEntity<Background>();
    }
};

struct Application : public age::Application {
    Application(const std::string& name, int width, int height)
        : age::Application(name, width, height) {}

    virtual void onCreate() override {
        age::FontInfo info;
        info.loadChars = "button1hello23";

        loadFont(age::getAssetPath("Courier.dfont"), "courier", info);
        loadTexture(age::getAssetPath("mountains.png"), "mountains");
        loadTexture(age::getAssetPath("yoda.jpg"), "yoda");

        auto scene = new TestScene();
        scene->create(this);
        setActiveScene(scene);
    }

    virtual void onEvent(age::Event e) override {
        if (e == age::event::KEY) {
            auto s = e.getStructure<age::event::Key>();
            if (s.action == GLFW_PRESS && s.key == GLFW_KEY_P) {
                stop();
            }
        } else if (e == age::event::MOUSE_BUTTON) {
            auto s = e.getStructure<age::event::MouseButton>();
            if (s.action == GLFW_PRESS) {
                std::cout << s.xPos << "; " << s.yPos << std::endl;
            }
        }
    }
};

int main(int argc, char* argv[]) {
    age::setAssetsPath(RESOURCE_PATH);

    Application* app = new Application("app", 800, 600);

    app->run();

    delete app;

    return 0;
}
