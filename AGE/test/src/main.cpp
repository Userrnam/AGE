#include <math.h>

#include "Application.hpp"

#include <glm/glm.hpp>
#include <iostream>
#include <string>

#include "Utils/utils.hpp"
#include "Graphics.hpp"

#include "TestTriangle.hpp"

#ifndef CMAKE_DEFINITION
#define RESOURCE_PATH ""
#endif

class ExampleLayer : public age::Layer {
    TestTriangle triangle;
    age::Text text;
    age::Font font;
    bool show_fps = true;

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
    virtual void onCoreConfig() override {
        age::setResourcePath(RESOURCE_PATH);
        age::core::CoreConfig config;
        config.window.width = 1200;
        config.window.height = 800;
        config.appInfo.name = "test";
        config.window.title = "hello";
        config.multisampling.sampleCount = VK_SAMPLE_COUNT_4_BIT;
        age::core::setCoreConfig(config);
        age::core::setClearColor({1, 1, 1, 1});
    }

    virtual void onCreate() override {
        pushLayer(new ExampleLayer());
    }

    virtual void onDestroy() override {

    }
};

int main(int argc, char* argv[]) {
    Application app;
    app.run();

    return 0;
}
