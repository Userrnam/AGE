#pragma once

// #include <memory>

// #include "Graphics/Core/CoreLayer.hpp"
#include "Layer.hpp"
#include <glm/glm.hpp>

namespace age {

class Application {
    std::vector<Layer*> m_layers;

    void update();
    void updateCommandBuffers();
protected:
    inline void pushLayer(Layer* layer) {
        m_layers.push_back(layer);
    }

    inline Layer* popLayer() {
        Layer* out = m_layers.back();
        m_layers.pop_back();
        return out;
    }

    // View defaultView;

    // glm::vec2 getWindowSize();

    virtual void onCoreConfig() {}
    virtual void onCreate() {}
    virtual void onDestroy() {}

    // // FIXME: rename Warning: draw must not be empty
    // virtual void draw(int i);
    // virtual void onCreate() {}
    // virtual void onUpdate(float elapsedTime) {}
    // virtual void onDelete() {}

public:
    ~Application();
    // void create();
    void run();
};

} // namespace age
