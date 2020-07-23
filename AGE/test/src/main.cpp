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

    virtual void onCreate() override {
        m_views.push_back(age::View());
        m_views[0].init();
        triangle.create(m_views[0]);

        // problem: store extra data (buffers etc)?
        // make some component for extra data
        std::vector<age::Drawable> targets = { triangle };

        parent->m_renderer.render(targets);
    }

    virtual void onUpdate(float elapsedTime) override {
        
    }

public:
    TestScene(age::Application* app) : age::Scene(app) {}
};

class Application : public age::Application {
    TestTriangle triangle;

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
            if (s.action == GLFW_PRESS && s.key == GLFW_KEY_S) {
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
        triangle.destroy();
    }
};

int main(int argc, char* argv[]) {
    Application app;
    app.run();

    return 0;
}
