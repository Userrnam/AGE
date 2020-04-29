#include <math.h>

#include "Application.hpp"

#include <glm/glm.hpp>

#include "Graphics/Core/coreAPI.hpp"
#include "utils.hpp"

class Application : public age::Application {
    double counter = 0;

    void onConfigure() override {
        age::core::CoreConfig config;
        config.window.width = 800;
        config.window.height = 600;
        config.appInfo.name = (char*)"test";
        config.window.title = (char*)"hello";
        age::core::setCoreConfig(config);
        age::core::setClearColor({1, 1, 1, 1});
    }

    void onUpdate(float elapsedTime) override {
        age::core::setClearColor({1, (1.0 + sin(counter)) / 2.0, 1, 1});
        updateCommandBuffers();
        counter += 2 * elapsedTime;
    }
};

int main() {
    Application app;
    app.init();
    app.run();

    return 0;
}
