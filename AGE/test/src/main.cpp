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
#include "TestRectangle.hpp"
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
    TestRectangle rect;
    age::Text text;
    age::Text text2;
    age::Font font;
    glm::vec2 move = {};
    float rotate = 0.0f;

    virtual void onCreate() override {
        font.load(age::getResourcePath("Courier.dfont"), parent->getDefaultSamplerCopy());

        // create view
        m_views.push_back(age::View());
        m_views[0].create();

        text.create(m_views[0], font);
        text.setText("Hello world");
        text.setColor({1, 1, 1, 1});
        text.uploadMapData();

        text2.create(m_views[0], font);
        text2.setText("Hell");
        text2.setColor({1, 0, 0, 1});
        text2.move(1200, 800);
        text2.uploadMapData();

        triangle.create(m_views[0]);
        rect.create(m_views[0]);

        auto entity3 = registry.create();
        registry.emplace<age::Drawable>(entity3, text2);

        auto entity = registry.create();
        registry.emplace<age::Drawable>(entity, text);

        auto entity2 = registry.create();
        registry.emplace<age::Drawable>(entity2, triangle);

        auto e = registry.create();
        registry.emplace<age::Drawable>(e, rect);

        auto view = registry.view<age::Drawable>();
        std::vector<age::Drawable> targets;
        targets.resize(view.size());
        for (size_t i = 0; i < view.size(); ++i) {
            targets[i] = view.get(view[i]);
        }
        parent->render({ age::RenderPack(m_views[0], targets) });
    }

    virtual void onDestroy() override {
        triangle.destroy();
        rect.destroy();
        text.destroy();
        font.destroy();
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
};

#include "Graphics/Components/TransformComponent.hpp"
#include "Graphics/Components/ColorComponent.hpp"
#include "Graphics/Components/TextureComponent.hpp"
#include "Graphics/Components/TexCoordsComponent.hpp"
#include "Containers/DynamicBuffer.hpp"

int main(int argc, char* argv[]) {
    Application app;
    app.create();

    age::TransformComponent transform;
    age::ColorComponent color;
    age::TextureComponent tex;
    age::TexCoordsComponent texCoord;

    age::ShaderBuilder shaderBuilder;
    shaderBuilder.generateVertexShaderSource(transform, color, texCoord, tex);
    shaderBuilder.saveShader("temp.vert");
    shaderBuilder.generateFragmentShaderSource(transform, color, texCoord, tex);
    shaderBuilder.saveShader("temp.frag");

    app.run();
    app.destroy();

    return 0;
}
