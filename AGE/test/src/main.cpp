#include <math.h>

#include "Application.hpp"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>

#include "Utils/utils.hpp"
#include "Graphics.hpp"
#include "Audio.hpp"
#include "Animation.hpp"

#include "Scene.hpp"

#ifndef CMAKE_DEFINITION
#define RESOURCE_PATH ""
#endif

#include "RectController.hpp"
#include "HelloText.hpp"
#include "Background.hpp"
#include "Triangle.hpp"
#include "Button.hpp"
#include "Graphics/PositionManager.hpp"
#include "Graphics/View/ViewManager.hpp"
#include "Graphics/Core/Core.hpp"

class TestScene : public age::Scene {
    virtual void onUpdate(float elapsedTime) override {
        std::string s = "fps: " + std::to_string(getApplication()->getFps());
        getApplication()->setWindowTitle(s);
    }

    void createUIBlock() {
        auto button1 = createEntity<Button>("button1", age::Vector2f(100, 100));
        auto button2 = createEntity<Button>("button2", age::Vector2f(500, 500));
        auto button3 = createEntity<Button>("button3", age::Vector2f(100, 500));

        age::UIBlock uiBlock;
        uiBlock.create(3);

        age::UIManager::addBlock(
            uiBlock
            .addButton(std::get<1>(button1))
            .addButton(std::get<1>(button2))
            .addButton(std::get<1>(button3))
             .alignVertically()
        );
    }

    virtual void onCreate() override {
        createEntity<HelloText>();
        createUIBlock();
        createEntity<RectController>();
        createStaticEntity<Triangle>();
        createStaticEntity<Background>();
    }
};

class Application : public age::Application {
    virtual void onCoreConfig() override {
        age::setResourcePath(RESOURCE_PATH);

        age::config::setApplicationName("app");
        age::config::setApplicationVersion(VK_MAKE_VERSION(1,0,0));
        age::config::setDebugEnable(true);
        age::config::setSampleCount(VK_SAMPLE_COUNT_4_BIT);
        age::config::setWindowProperties(
            age::config::WindowProperties()
            .setResizeEnable(true)
            .setSize(800, 600)
            .setTitle("app")
        );
    }

    virtual void onCreate() override {
        loadFont(age::getResourcePath("Courier.dfont"), "courier");
        loadTexture(age::getResourcePath("mountains.png"), "mountains");
        loadTexture(age::getResourcePath("yoda.jpg"), "yoda");

        auto& view = age::ViewManager::getView(age::hash("default"));

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
    Application app;
    app.create();
    app.run();
    app.destroy();

    return 0;
}
