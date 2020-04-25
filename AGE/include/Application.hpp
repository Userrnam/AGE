#pragma once

namespace age {

class Application {
protected:
    void updateCommandBuffers();

    // FIXME: rename Warning: draw must not be empty
    virtual void draw(int i);
    virtual void onConfigure();
    virtual void onCreate();
    virtual void onUpdate() {}

public:
    ~Application();
    void init();
    void run();
};

} // namespace age
