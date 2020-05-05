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

#ifndef CMAKE_DEFINITION
#define RESOURCE_PATH ""
#endif

class Application : public age::Application {
    age::Texture tex;
    age::Rectangle rect;
    glm::vec2 winSize;

    double counter = 0;

    void draw(int i) override {
        age::core::cmd::clear(i);
        rect.draw(i);
    }

    void onCreate() override {
        winSize = getWindowSize();
        tex.create(age::getResourcePath("mountains.png"));
        rect.create();
        rect.setColor({1,0,0,1});
        rect.setScale({ winSize.x / 4, winSize.x / 2 });
        rect.setPosition({ winSize.x / 2, winSize.y / 2 });
        // rect.setOrigin({ winSize.x / 8, winSize.x / 4 });
        rect.upload();
    }

    void onDelete() override {
        tex.destroy();
        rect.destroy();
    }

    void onUpdate(float elapsedTime) override {
        counter += 2*elapsedTime;
        rect.setRotation(counter);
        rect.upload();
    }
};

int main(int argc, char* argv[]) {
    std::string path = std::string(argv[0]);
    std::cout << path << std::endl;

    age::setResourcePath(RESOURCE_PATH);

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
