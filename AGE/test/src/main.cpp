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

#ifndef CMAKE_DEFINITION
#define RESOURCE_PATH ""
#endif

class ExampleLayer : public age::Layer {
    TestTriangle triangle;
    age::Text text;
    age::Font font;

    virtual void draw(int i) override {
        clearWindow(i);
        triangle.draw(i);
        text.draw(i);
    }

    virtual void onCreate() override {
        // font.load(age::getResourcePath("BraveSt.ttf"));
        font.load(age::getResourcePath("Courier.dfont"));
        triangle.create(this);
        text.create(this, font);
        text.setPosition(100, 100);
        text.setColor(1, 1, 0, 1);

        text.uploadMapData();
    }

    virtual void onDestroy() override {
        font.destroy();
        text.destroy();
        triangle.destroy();
    }

    virtual void onUpdate(float elapsedTime) override {
        text.rotate(elapsedTime);
        text.setOrigin(0.5, 0.5);
        text.uploadMapData();
        text.setText("fps: " + std::to_string(1.0/elapsedTime));
    }

public:
    ExampleLayer(const age::Viewport& viewport = {}) : Layer(viewport) {}
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
            .setResizeEnable(false)
            .setSize(800, 600)
            .setTitle("app")
        );
    }

    virtual void onCreate() override {
        // audio test
        age::Listener::setPosition(0, 0, 1);
        age::Listener::setVelocity(0, 0, 0);
        age::Listener::setOrientation(age::Orientation());

        sound.create();
        sound.load(age::getResourcePath("test.wav"));

        source.create();
        source.setPitch(1);
        source.setGain(0.5);
        source.setPosition(0, 0, 0);
        source.setVelocity(0, 0, 0);
        source.setLooping(true);
        source.setSound(&sound);

        source.play();

        pushLayer(new ExampleLayer());
    }

    virtual void onDestroy() override {
        source.destroy();
        sound.destroy();
    }
};

int main(int argc, char* argv[]) {

    age::Matrix2f mat1 = {
        1.0, 2.0,
        3.0, 4.0
    };

    age::Matrix2f mat2 = {
        1.0, 5.0,
        4.0, 2.0
    };

    std::cout << mat1 * mat2;

    // Application app;
    // app.run();

    return 0;
}
