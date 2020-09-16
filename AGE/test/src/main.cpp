#include <math.h>

#include "Application.hpp"

#include <glm/glm.hpp>
#include <string>

#include "Utils/utils.hpp"
#include "Graphics.hpp"
#include "Audio.hpp"

#include "Scene.hpp"
#include <GLFW/glfw3.h>

#ifndef CMAKE_DEFINITION
#define RESOURCE_PATH ""
#endif

#include "RectController.hpp"
#include "HelloText.hpp"
#include "Background.hpp"
#include "Triangle.hpp"

class TestScene : public age::Scene {
    virtual void onCreate() override {
        createStaticEntity<HelloText>();
        createEntity<RectController>();
        createStaticEntity<Triangle>();
        createStaticEntity<Background>();

        // todo: move it somewhere else
        auto view = m_registry.view<age::Drawable>();
        std::vector<age::Drawable> targets;
        targets.resize(view.size());
        for (size_t i = 0; i < view.size(); ++i) {
            targets[i] = view.get(view[i]);
        }

        age::Renderer::render(targets);
    }
};

class Application : public age::Application {
    age::Sound sound;
    age::SoundSource source;

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

        sound.create();
        sound.load(age::getResourcePath("test.wav"));
        source.create();
        source.setLooping(true);
        source.setSound(sound);
        source.play();

        auto scene = new TestScene();
        scene->create(this);
        setActiveScene(scene);
    }

    virtual void onDestroy() override {
        source.destroy();
        sound.destroy();
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
