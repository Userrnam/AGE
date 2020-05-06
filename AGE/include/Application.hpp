#pragma once

#include "Graphics/Core/CoreLayer.hpp"
#include "Graphics/View.hpp"
#include <glm/glm.hpp>

namespace age {

class Application : public core::CoreLayer {
protected:
    View defaultView;

    void updateCommandBuffers();
    glm::vec2 getWindowSize();

    // FIXME: rename Warning: draw must not be empty
    virtual void draw(int i);
    virtual void onCreate() {}
    virtual void onUpdate(float elapsedTime) {}
    virtual void onDelete() {}

public:
    void create();
    void run();
};

} // namespace age
