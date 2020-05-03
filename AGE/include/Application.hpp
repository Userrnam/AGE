#pragma once

#include "Graphics/Core/CoreLayer.hpp"
#include <glm/glm.hpp>

namespace age {

class Application : public core::CoreLayer {
protected:
    void updateCommandBuffers();
    glm::vec2 getWindowSize();

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
