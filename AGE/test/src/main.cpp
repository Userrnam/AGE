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

#ifndef CMAKE_DEFINITION
#define RESOURCE_PATH ""
#endif

// how to update position for physics, renderer, audio simultaniously?
// positions are updated by physics engine, so everything has outdated data
// 1. run physics simulation
// 2. update positions for audio and drawable

/*

all entities for scene created at once, than I can use

physics component -> position, events
position, size (used by renderer and audio)
drawable
[pd] sound
[pd] ubo component.update()
descriptor

some extra data (texture)

everything depends on position

*/

// it's scene + game logic, maybe change it?
class TestScene : public age::Scene {
    TestTriangle triangle;
    age::Text text;
    age::Font font;
    glm::vec2 move = {};

    virtual void onCreate() override {
        font.load(age::getResourcePath("Courier.dfont"), parent->getDefaultSamplerCopy());

        m_views.push_back(age::View());
        m_views[0].init();

        text.create(m_views[0], font);
        text.setText("Hello world");
        text.setColor({1, 1, 1, 1});
        text.uploadMapData();

        triangle.create(m_views[0]);

        auto entity = registry.create();
        registry.emplace<age::Drawable>(entity, text);

        auto entity2 = registry.create();
        registry.emplace<age::Drawable>(entity2, triangle);

        auto view = registry.view<age::Drawable>();
        std::vector<age::Drawable> targets;
        targets.resize(view.size());
        for (size_t i = 0; i < view.size(); ++i) {
            targets[i] = view.get(view[i]);
        }
        parent->render(targets);
    }

    virtual void onDestroy() override {
        triangle.destroy();
        text.destroy();
        font.destroy();
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
                
                default:
                    break;
                }
            } else if (e.action == GLFW_RELEASE) {
                switch (e.key) {
                case GLFW_KEY_W: case GLFW_KEY_S: move.y = 0; break;
                case GLFW_KEY_D: case GLFW_KEY_A: move.x = 0; break;
                
                default:
                    break;
                }
            }            
        }
    }

    virtual void onUpdate(float elapsedTime) override {
        if (move != glm::vec2(0,0)) {
            text.move(move);
            text.uploadMapData();
        }
    }

public:
    TestScene(age::Application* app) : age::Scene(app) {}
};

// dont know why but mac says app quit unexpectedly on exit
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
        setActiveScene(new TestScene(this));
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

    virtual void onDestroy() override {
        for (auto view : m_views) {
            view.destroy();
        }
    }
};

#include "Graphics/Components/TransformComponent.hpp"
#include "Graphics/Components/ColorComponent.hpp"
#include "Graphics/Components/TextureComponent.hpp"
#include "Containers/DynamicBuffer.hpp"

int main(int argc, char* argv[]) {
    Application app;
    app.create();

    age::TransformComponent transform;
    age::ColorComponent color;
    age::TextureComponent tex;
    age::TextureCoordComponent texCoord;

    std::vector<age::IGraphicsComponent*> components = {
        &transform,
        &color,
        &tex,
        &texCoord
    };

    age::ShaderBuilder shaderBuilder;

    auto shader = shaderBuilder.compileFragmentShader(components);
    shader.destroy();

    app.run();
    app.destroy();

    return 0;
}
