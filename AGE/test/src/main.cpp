#include <math.h>

#include "Application.hpp"

#include <glm/glm.hpp>
#include <string>

#include "Utils/utils.hpp"
#include "Graphics.hpp"
#include "Audio.hpp"

#include "Scene.hpp"
#include <GlFW/glfw3.h>

#ifndef CMAKE_DEFINITION
#define RESOURCE_PATH ""
#endif

#include "RectController.hpp"
#include "HelloText.hpp"
#include "TestTriangle.hpp"

class TestScene : public age::Scene {
    TestTriangle triangle;
    glm::vec2 move = {};
    float rotate = 0.0f;

    virtual void onCreate() override {
        triangle.create();

        // create text
        auto entity = createEntity();
        auto script = entity.addComponentNoCreate<age::ScriptComponent*>(new HelloText());
        script->create(entity);

        entity = createEntity();
        script = entity.addComponentNoCreate<age::ScriptComponent*>(new RectController());
        script->create(entity);

        entity = createEntity();
        entity.addComponentNoCreate<age::Drawable>(triangle);

        // add background
        {
            auto background = createEntity();
            background.addComponent<age::Drawable>(age::RECTANGLE_SHAPE, 
                background.addComponent<age::StorageComponent<age::Transform>>(age::Transformable().setScale(1600, 1200).getTransform()),
                background.addComponent<age::StorageComponent<age::Color>>(glm::vec4(1, 0, 0, 1))
            );
        }

        auto view = m_registry.view<age::Drawable>();
        std::vector<age::Drawable> targets;
        targets.resize(view.size());
        for (size_t i = 0; i < view.size(); ++i) {
            targets[i] = view.get(view[i]);
        }

        age::Renderer::render(targets);
    }

    virtual void onDestroy() override {
        triangle.destroy();
    }

    virtual void onEvent(age::Event event) override {
        if (event == age::event::KEY) {
            auto e = event.getStructure<age::event::Key>();
            if (e.action == GLFW_PRESS || e.action == GLFW_REPEAT) {
                switch (e.key) {
                case GLFW_KEY_W: move.y = 10; break;
                case GLFW_KEY_S: move.y = -10; break;
                case GLFW_KEY_D: move.x = 10; break;
                case GLFW_KEY_A: move.x = -10; break;

                case GLFW_KEY_J: rotate = 0.05; break;
                case GLFW_KEY_K: rotate = -0.05; break;
                
                default:
                    break;
                }
            } else if (e.action == GLFW_RELEASE) {
                switch (e.key) {
                case GLFW_KEY_W: case GLFW_KEY_S: move.y = 0; break;
                case GLFW_KEY_D: case GLFW_KEY_A: move.x = 0; break;
                case GLFW_KEY_J: case GLFW_KEY_K: rotate = 0; break;
                
                default:
                    break;
                }
            }
        }
    }

    virtual void onUpdate(float elapsedTime) override {

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
            .setResizeEnable(false)
            .setSize(800, 600)
            .setTitle("app")
        );
    }

    virtual void onCreate() override {
        loadFont(age::getResourcePath("Courier.dfont"), "courier");
        loadTexture(age::getResourcePath("mountains.png"), "mountains");
        loadTexture(age::getResourcePath("yoda.jpg"), "yoda");

        auto scene = new TestScene();
        scene->create(this);
        setActiveScene(scene);
    }

    virtual void onEvent(age::Event e) override {
        switch (e) {

        case age::event::MOUSE_BUTTON: {
            auto s = e.getStructure<age::event::MouseButton>();
            if (s.action == GLFW_PRESS && s.button == GLFW_MOUSE_BUTTON_1) {
                std::cout << "pos: (" << s.xPos << "; " << s.yPos << ")\n";
            }
            break;
        }

        case age::event::KEY: {
            auto s = e.getStructure<age::event::Key>();
            if (s.action == GLFW_PRESS && s.key == GLFW_KEY_P) {
                stop();
            }
            break;
        }
        default:
            break;
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
