#include <math.h>

#include "Application.hpp"

#include <glm/glm.hpp>
#include <iostream>
#include <string>

#include "Utils/utils.hpp"
#include "Graphics.hpp"
#include "Audio.hpp"
#include "Math.hpp"

#include "TestTriangle.hpp"
#include "Scene.hpp"
#include <GlFW/glfw3.h>

#include "Graphics/ShaderBuilder.hpp"

#include "Graphics/Components/TransformComponent.hpp"
#include "Graphics/Components/ColorComponent.hpp"
#include "Graphics/Components/TextureComponent.hpp"
#include "Graphics/Components/TexCoordsComponent.hpp"
#include "Graphics/Components/TileMapComponent.hpp"
#include "Graphics/Components/BundleComponent.hpp"
#include "Containers/DynamicBuffer.hpp"

#ifndef CMAKE_DEFINITION
#define RESOURCE_PATH ""
#endif

struct RectController : public age::ScriptComponent {
    // using ComponentVariables = age::BundleComponent<age::Color, age::Transform>;

    glm::vec2 move = {};
    age::Transformable transformable;
    // ComponentVariables variables;
    age::StorageComponent<age::Transform> transform;
    age::ArrayComponent<age::Color, age::PER_VERTEX> colors;
    const float speed = 1.0f;

    virtual void onCreate() override {
        transformable.setScale(100, 100);

        transform.create(transformable.setScale(100, 100).getTransform());
        colors.create(4);
        colors.add({{1, 0, 0, 1}});
        colors.add({{0, 1, 0, 1}});
        colors.add({{0, 0, 1, 1}});
        colors.add({{0, 1, 1, 1}});
        colors.upload();

        addComponent<age::Drawable>(
            age::RECTANGLE_SHAPE,
            transform,
            colors
        );
    }

    virtual void onEvent(age::Event event) override {
        if (event == age::event::KEY) {
            auto e = event.getStructure<age::event::Key>();
            if (e.action == GLFW_PRESS) {
                switch (e.key) {
                    case GLFW_KEY_UP: move.y = 10; break;
                    case GLFW_KEY_DOWN: move.y = -10; break;
                    case GLFW_KEY_RIGHT: move.x = 10; break;
                    case GLFW_KEY_LEFT: move.x = -10; break;
                    default:
                    break;
                }
            } else if (e.action == GLFW_RELEASE) {
                switch (e.key) {
                case GLFW_KEY_UP: case GLFW_KEY_DOWN: move.y = 0; break;
                case GLFW_KEY_LEFT: case GLFW_KEY_RIGHT: move.x = 0; break;
                
                default:
                    break;
                }
            }
        }
    }

    virtual void onUpdate(float elapsedTime) override {
        if (move != glm::vec2(0.0)) {
            transformable.move(move.x * speed, move.y * speed);
            transform.set(transformable.getTransform());
        }
    }

    virtual void onDestroy() override {
        transform.destroy();
        colors.destroy();
        getComponent<age::Drawable>().destroy();
    }
};

class TestScene : public age::Scene {
    TestTriangle triangle;
    age::Text text;
    age::Text text2;
    glm::vec2 move = {};
    float rotate = 0.0f;

    virtual void onCreate() override {
        auto e = createEntity();

        text.create(getFont("courier"));
        text.setText("Hello world");
        text.setColor({1, 1, 1, 1});
        text.uploadMapData();

        text2.create(getFont("courier"));
        text2.setText("Hell");
        text2.setColor({1, 0, 0, 1});
        text2.move(1200, 800);
        text2.uploadMapData();

        triangle.create();

        auto entity = createEntity();
        entity.addComponentNoCreate<age::Drawable>(text2);

        entity = createEntity();
        entity.addComponentNoCreate<age::Drawable>(text);

        // create rectangle
        entity = createEntity();
        auto script = entity.addComponentNoCreate<age::ScriptComponent*>(new RectController());
        script->create(entity);

        entity = createEntity();
        entity.addComponentNoCreate<age::Drawable>(triangle);

        // add background
        {
            auto background = createEntity();
            // FIXME: if we swap color and transform it will not be rendered
            background.addComponent<age::Drawable>(age::RECTANGLE_SHAPE, 
                background.addComponent<age::StorageComponent<age::Color>>(glm::vec4(1, 0, 0, 1)),
                background.addComponent<age::StorageComponent<age::Transform>>(age::Transformable().setScale(1600, 1200).getTransform())
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
        text.destroy();
        text2.destroy();
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
        if (move != glm::vec2(0,0) || rotate != 0) {
            text.move(move);
            text.rotate(rotate);
            text.uploadMapData();
        }
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
