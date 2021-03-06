#include "CoreConfiguration.hpp"
#include "CoreConfig.hpp"

namespace age::core {
    extern CoreConfig coreConfig;
}

namespace age::config {

void enableFeatures(Features features) {
    if (features & SAMPLE_RATE_SHADING) {
        core::coreConfig.multisampling.sampleRateShading = true;
    }
}

void setApplicationName(const std::string& name) {
    core::coreConfig.appInfo.name = name;
}

void setApplicationVersion(uint32_t version) {
    core::coreConfig.appInfo.version = version;
}

void setWindowProperties(const WindowProperties& properties) {
    core::coreConfig.window.title = properties.m_title;
    core::coreConfig.window.resizable = properties.m_resizable;
    core::coreConfig.window.width = properties.m_width;
    core::coreConfig.window.height = properties.m_height;
}

void setSampleCount(VkSampleCountFlagBits count) {
    core::coreConfig.multisampling.sampleCount = count;
}

void setDebugEnable(bool flag) {
    core::coreConfig.debugEnable = flag;
}

} // namespace age::config
