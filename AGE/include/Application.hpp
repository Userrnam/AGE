#pragma once

namespace age {

class Application {
protected:
    void updateCommandBuffers();

    // FIXME: rename Warning: draw must not be empty
    virtual void draw(int i);
    virtual void onCreate();
    virtual void onUpdate(float elapsedTime) {}

public:
    Application();
    ~Application();
    void run();
};

} // namespace age
