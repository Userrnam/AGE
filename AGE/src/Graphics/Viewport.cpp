#include "Viewport.hpp"
#include "Core/Core.hpp"

namespace age {

Viewport currentViewport;

void setViewport(const Viewport& viewport) {
    currentViewport = viewport;
}

void setDefaultViewport() {
    currentViewport.x = 0;
    currentViewport.y = 0;
    currentViewport.width  = static_cast<float>(core::apiCore.swapchain.extent.width);
    currentViewport.height = static_cast<float>(core::apiCore.swapchain.extent.height);
}

} // namespace age
