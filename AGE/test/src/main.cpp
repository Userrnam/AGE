#include <math.h>

#include "Application.hpp"

#include <glm/glm.hpp>
#include <iostream>
#include <string>

#include "Graphics/Core/coreAPI.hpp"
#include "Utils/utils.hpp"
#include "Graphics/Rectangle.hpp"
#include "Graphics/Core/Command.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Viewport.hpp"
#include "Graphics/View.hpp"

#ifndef CMAKE_DEFINITION
#define RESOURCE_PATH ""
#endif

class Application : public age::Application {
    age::Texture tex;
    age::Rectangle rect;
    glm::vec2 winSize;
    glm::vec2 rectSize;

    std::vector<age::Rectangle> inheritedRects;

    double counter = 0;

    void draw(int i) override {
        age::core::cmd::clear(i);
        rect.draw(i);
        for (auto& r : inheritedRects) {
            r.draw(i);
        }
    }

    void onCreate() override {
        winSize = getWindowSize();
        tex.create(age::getResourcePath("mountains.png"));
        rect.create(defaultView);

        rect.setColor({1,0,0,1});
        rect.setScale({ winSize.x / 4, winSize.x / 2 });
        rect.setPosition({ winSize.x / 2, winSize.y / 2 });
        rect.setOrigin({ 0.5, 0.5 });
        rect.upload();

        rectSize = rect.getScale();

        inheritedRects.resize(100);
        for (auto& r : inheritedRects) {
            r.create(rect);
            r.setColor({0,1,1,1});
            r.setScale({ winSize.x / 8, winSize.x / 8 });
            r.setPosition({ winSize.x / 4, winSize.x / 4 });
            r.upload();
        }

    }

    void onDelete() override {
        tex.destroy();
        rect.destroy();
        for (auto& r : inheritedRects) {
            r.destroy();
        }
    }

    void onUpdate(float elapsedTime) override {
        counter += elapsedTime;


        rect.setRotation(counter);
        rect.setScale(rectSize.x * (1 + cos(counter))/2, rectSize.y * (1 + cos(counter))/2);
        rect.upload();

        defaultView.setRotation(counter / 2);
        defaultView.setScale((1 + sin(counter))/2, (1 + sin(counter))/2);
        defaultView.upload();

        for (auto& r : inheritedRects) {
            r.setColor({ 1 + cos(counter)/2, 1+sin(counter)/2, 0, 1 });
            r.upload();
        }

        std::cout << "fps: " << 1. / elapsedTime << '\n';
    }
};

int main(int argc, char* argv[]) {
    age::setResourcePath(RESOURCE_PATH);

    age::core::CoreConfig config;
    config.window.width = 800;
    config.window.height = 600;
    config.appInfo.name = "test";
    config.window.title = "hello";
    config.multisampling.sampleCount = age::core::SAMPLE_COUNT_4;
    age::core::setCoreConfig(config);
    age::core::setClearColor({1, 1, 1, 1});

    Application app;
    app.create();
    app.run();

    return 0;
}
