#pragma once

#include <vulkan/vulkan.h>

#include <stdint.h>
#include <string>

namespace age::config {

typedef uint32_t Features;

enum Feature {
    SAMPLE_RATE_SHADING = 1
};

class WindowProperties {
    std::string m_title;
    bool m_resizable = false;
    int m_width = 0;
    int m_height = 0;
    friend void setWindowProperties(const WindowProperties& properties);
public:
    inline WindowProperties& setTitle(const std::string& title) {
        m_title = title;
        return *this;
    }

    inline WindowProperties& setSize(int width, int height) {
        m_width = width;
        m_height = height;
        return *this;
    }

    inline WindowProperties& setResizeEnable(bool flag) {
        m_resizable = flag;
        return *this;
    }
};

void enableFeatures(Features features);

void setApplicationName(const std::string& name);
void setApplicationVersion(uint32_t version);

void setWindowProperties(const WindowProperties& properties);

void setSampleCount(VkSampleCountFlagBits count);

void setDebugEnable(bool flag);

} // namespace age::config
