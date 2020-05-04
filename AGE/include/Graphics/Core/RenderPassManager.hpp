#pragma once

#include <vulkan/vulkan.h>

#include "RenderPassRef.hpp"

namespace age::core {

RenderPassRef* requestRenderPass(RenderPassConfig rpc);

} // namespace age::core
