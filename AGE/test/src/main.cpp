#include <math.h>

#include "Application.hpp"

#include <glm/glm.hpp>

#include "Graphics/Core/coreAPI.hpp"
#include "utils.hpp"
// #include "Graphics/Rectangle.hpp"
#include "Graphics/Core/Command.hpp"

class Application : public age::Application {
    double counter = 0;
    // age::Rectangle rect;

    void draw(int i) override {
        age::core::cmd::clear(i);
        // rect.draw(i);
    }

    void onCreate() override {
        updateCommandBuffers();
    }

    void onUpdate(float elapsedTime) override {
        age::core::setClearColor({1, (1.0 + sin(counter)) / 2.0, 1, 1});
        updateCommandBuffers();
        counter += 2 * elapsedTime;
    }
};

int main() {
    age::core::CoreConfig config;
    config.window.width = 800;
    config.window.height = 600;
    config.appInfo.name = (char*)"test";
    config.window.title = (char*)"hello";
    age::core::setCoreConfig(config);
    age::core::setClearColor({1, 1, 1, 1});

    Application app;
    app.run();

    return 0;
}
