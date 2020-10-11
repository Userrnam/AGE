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

class TestScene : public age::Scene {
    virtual void onUpdate(float elapsedTime) override {
        std::string s = "fps: " + std::to_string(getApplication()->getFps());
        getApplication()->setWindowTitle(s);
    }

    virtual void onCreate() override {
        createEntity<HelloText>();
        createEntity<Button>();
        createEntity<RectController>();
        createStaticEntity<Triangle>();
        createStaticEntity<Background>();

        age::Positionable p;
        p.pos = { 0, 0 };
        p.size = { 800, 600 };
        auto renderEntities = age::PositionManager::getVisibleEntities(p);

        std::vector<age::Drawable> targets;
        targets.resize(renderEntities.size());
        for (int i = 0; i < targets.size(); ++i) {
            targets[i] = m_registry.get<age::Drawable>(renderEntities[i]);
        }

        // fixme: move this from user code
        // fixme: get visible targets before rendering
        age::Renderer::render(targets);
    }
};

class Application : public age::Application {
    // age::Sound sound;
    // age::SoundSource source;

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

        // sound.create();
        // sound.load(age::getResourcePath("test.wav"));
        // source.create();
        // source.setLooping(true);
        // source.setSound(sound);
        // source.play();

        auto scene = new TestScene();
        scene->create(this);
        setActiveScene(scene);
    }

    virtual void onDestroy() override {
        // source.destroy();
        // sound.destroy();
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
