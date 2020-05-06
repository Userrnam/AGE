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
    age::View dynamicView;
    age::Texture tex;
    age::Texture emptyTex;
    age::Rectangle rect;
    age::Rectangle background;
    age::Rectangle emptyRect;

    glm::vec2 winSize;
    glm::vec2 rectSize;

    std::vector<age::Rectangle> inheritedRects;

    double counter = 0;

    void draw(int i) override {
        background.draw(i);
        rect.draw(i);
        for (auto& r : inheritedRects) {
            r.draw(i);
        }
    }

    void onCreate() override {
        winSize = getWindowSize();
        tex.create(age::getResourcePath("mountains.png"));
        rect.create(dynamicView);

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

        background.create(defaultView, tex);
        background.setScale(winSize);
        background.setPosition(0, 0);
        background.setColor(1, 1, 1, 1);
        background.upload();
    }

    void onDelete() override {
        
    }

    void onUpdate(float elapsedTime) override {
        counter += elapsedTime;


        rect.setRotation(counter);
        rect.setScale(rectSize.x * (1 + cos(counter))/2, rectSize.y * (1 + cos(counter))/2);
        rect.upload();

        dynamicView.setRotation(counter / 2);
        dynamicView.setScale((1 + sin(counter))/2, (1 + sin(counter))/2);
        dynamicView.upload();

        for (auto& r : inheritedRects) {
            r.setColor({ 1 + cos(counter)/2, 1+sin(counter)/2, 0, 1 });
            r.upload();
        }

        // std::cout << "fps: " << 1. / elapsedTime << '\n';
    }
};

int main(int argc, char* argv[]) {
    age::setResourcePath(RESOURCE_PATH);

    age::core::CoreConfig config;
    config.window.width = 948;
    config.window.height = 481;
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
