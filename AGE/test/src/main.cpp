#include <math.h>

#include "Application.hpp"

#include <glm/glm.hpp>
#include <iostream>

#include "Graphics/Core/coreAPI.hpp"
#include "Utils/utils.hpp"
#include "Graphics/Rectangle.hpp"
#include "Graphics/Core/Command.hpp"

class Application : public age::Application {
    double counter = 0;
    std::vector<age::Rectangle> rects;

    void draw(int i) override {
        age::core::cmd::clear(i);
        for (auto& r : rects) {
            r.draw(i);
        }
    }

    void onCreate() override {
        auto winSize = getWindowSize();
        float size = winSize.y / 24.;
        float pos = winSize.x / 20.;
        for (int i = 0; i < 10; ++i) {
            age::Rectangle r;
            r.create();
            r.setSize({ size * i, size * i });
            r.setColor({ 0.05 * i, 0.05 * i, 0.05*i, 1.0});
            r.setPosition({i * pos, i * pos});
            r.upload();

            rects.push_back(r);
        }
        updateCommandBuffers();
    }

    void onDelete() override {
        for (auto& r : rects) {
            r.destroy();
        }
    }

    void onUpdate(float elapsedTime) override {
        counter += 2*elapsedTime;

        for (auto& r : rects) {
            auto pos = r.getPosition();
            auto size = r.getSize();
            r.setPosition({ 600 + 100 * (2 + sin(counter + pos.x / size.x / 5)), pos.y });
            r.upload();
        }
    }
};

int main(int argc, char* argv[]) {
    std::cout << argv[0] << "\n";
    // age::setResourcePath("/Users/antonkondratuk/Desktop/Vulkan/AGE/AGE/src/Graphics/Shaders/");

    age::core::CoreConfig config;
    config.window.width = 800;
    config.window.height = 600;
    config.appInfo.name = (char*)"test";
    config.window.title = (char*)"hello";
    age::core::setCoreConfig(config);
    age::core::setClearColor({1, 1, 1, 1});

    Application app;
    app.create();
    app.run();

    return 0;
}
