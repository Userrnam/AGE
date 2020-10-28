#include "Scenes/test/testScene.hpp"
#include "Scenes/mainMenu/menu.hpp"

#ifndef CMAKE_DEFINITION
#define RESOURCE_PATH ""
#endif

struct Application : public age::Application {
    Application(const std::string& name, int width, int height)
        : age::Application(name, width, height) {}

    virtual void onCreate() override {
        loadFont(age::getAssetPath("Courier.dfont"), "courier");
        loadTexture(age::getAssetPath("mountains.png"), "mountains");
        loadTexture(age::getAssetPath("yoda.jpg"), "yoda");

        // selectScene<MainMenu>();
        setActiveScene<TestScene>();
    }

    virtual void onEvent(age::Event e) override {
        if (e == age::event::KEY) {
            auto s = e.getStructure<age::event::Key>();
            if (s.action == GLFW_PRESS && s.key == GLFW_KEY_P) {
                stop();
            }
            // if (s.action == GLFW_PRESS && s.key == GLFW_KEY_R) {
            //     selectScene<TestScene>();
            // }
        }
    }
};

int main(int argc, char* argv[]) {
    age::setAssetsPath(RESOURCE_PATH);

    Application* app = new Application("app", 800, 600);

    app->run();

    delete app;

    return 0;
}