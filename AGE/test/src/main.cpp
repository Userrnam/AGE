#include <math.h>

#include "Application.hpp"

#include <glm/glm.hpp>
#include <iostream>
#include <string>

#include "Utils/utils.hpp"
#include "Graphics.hpp"

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

    age::RectangleFactory rFactory;
    std::vector<age::RectangleInstance> rInstances;

    age::TexturedRectangleFactory trFactory;
    std::vector<age::TexturedRectangleInstance> trInstances;

    age::Font font;
    age::Text text;

    double counter = 0;

    void draw(int i) override {
        background.draw(i);
        rect.draw(i);
        rFactory.draw(i);
        trFactory.draw(i);
        text.draw(i);
    }

    void onCreate() override {
        age::core::setClearColor({1, 0, 0, 1});
        winSize = getWindowSize();
        tex.create(age::getResourcePath("mountains.png"));

        // font.load(age::getResourcePath("BraveSt.ttf"), 100);
        font.load(age::getResourcePath("Courier.dfont"), 100);
        text.create(defaultView, font);
        text.setText("hello");

        trFactory.create(defaultView, 1, tex);
        trInstances.resize(1);
        for (auto& tr : trInstances) {
            trFactory.addChild(tr);
            tr.setPosition(0,0);
            tr.setScale(200, 100);
            glm::vec2 texCoords[] = {
                {0, 1},
                {1, 1},
                {1, 0},
                {0, 0},
            };
            tr.setTexCoords(texCoords);
            tr.updateTransform();
        }
        trFactory.upload();

        rFactory.create(defaultView, 100);
        // add children
        rInstances.resize(100);
        int i = 0;
        for (auto& r : rInstances) {
            rFactory.addChild(r);
            r.setColor(1, 0, 0, 1);
            r.setPosition(i, 0);
            r.setScale(100, 100);
            r.setRotation(static_cast<float>(i) * 0.001);
            r.updateTransform();
            i += 100;
        }
        rFactory.upload();

        rect.create(dynamicView, true);

        rect.setColor({1,0,0,0.5});
        rect.setScale({ winSize.x / 4, winSize.x / 2 });
        rect.setPosition({ winSize.x / 2, winSize.y / 2 });
        rect.setOrigin({ 0.5, 0.5 });
        rect.upload();

        rectSize = rect.getScale();

        background.create(defaultView, tex);
        background.setScale(winSize);
        background.setPosition(0, 0);
        background.setColor(1, 1, 1, 1);
        background.upload();
    }

    void onDelete() override {
        font.destroy();
        text.destroy();
        tex.destroy();
        background.destroy();
        rect.destroy();
        rFactory.destroy();
        trFactory.destroy();
    }

    void onUpdate(float elapsedTime) override {
        counter += elapsedTime;

        rect.setRotation(counter);
        rect.setScale(rectSize.x * (1 + cos(counter))/2, rectSize.y * (1 + cos(counter))/2);
        rect.upload();

        dynamicView.setRotation(counter / 2);
        dynamicView.setScale((1 + sin(counter))/2, (1 + sin(counter))/2);
        dynamicView.upload();

        for (auto& r : rInstances) {
            r.setColor(1+cos(counter*2)/2, 1+sin(counter*2)/2, 0.7, 0.5);
            r.rotate(0.01);
            r.updateTransform();
        }
        rFactory.upload();

        // std::cout << "fps: " << 1. / elapsedTime << '\n';
    }
};

int main(int argc, char* argv[]) {
    age::setResourcePath(RESOURCE_PATH);
    age::initFreeType();

    age::core::CoreConfig config;
    config.window.width = 948;
    config.window.height = 481;
    config.appInfo.name = "test";
    config.window.title = "hello";
    config.multisampling.sampleCount = VK_SAMPLE_COUNT_1_BIT;
    age::core::setCoreConfig(config);
    age::core::setClearColor({1, 1, 1, 1});

    Application app;
    app.create();
    app.run();

    return 0;
}
