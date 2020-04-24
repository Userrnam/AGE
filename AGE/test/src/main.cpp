#include "Application.hpp"

#include "Core.hpp"

class Application : public age::Application {
    void onConfigure() override {
        age::core::CoreConfig config;
        config.window.width = 800;
        config.window.height = 600;
        age::core::setCoreConfig(config);
        age::core::setClearColor({1, 1, 1, 1});
    }
};

int main() {
    Application app;
    app.init();
    app.run();

    return 0;
}
