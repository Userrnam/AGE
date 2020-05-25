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

    virtual void draw(int i) override {
        // clearWindow(i);
        triangle.draw(i);
        text.draw(i);
    }

    virtual void onCreate() override {
        // font.load(age::getResourcePath("Courier.dfont"), 100);
        font.load(age::getResourcePath("BraveSt.ttf"));
        text.create(this, font);
        text.setText("Hello, world");
        // text.move(100, 100);
        triangle.create(this);
    }

    virtual void onDestroy() override {
        font.destroy();
        text.destroy();
        triangle.destroy();
    }

    virtual void onUpdate(float elapsedTime) override {

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
