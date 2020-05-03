#pragma once

#include "Graphics/Core/CoreLayer.hpp"

namespace age {

class Application : public core::CoreLayer {
protected:
    void updateCommandBuffers();

    // FIXME: rename Warning: draw must not be empty
    virtual void draw(int i);
    virtual void onCreate();
    virtual void onUpdate(float elapsedTime) {}
    virtual void onDelete() {}

public:
    Application();
    void create();
    void run();
};

} // namespace age
