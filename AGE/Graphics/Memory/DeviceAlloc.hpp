#pragma once

#include <vulkan/vulkan.h>

#include "MemoryId.hpp"

namespace age::core::deviceAlloc {

MemoryId allocBuffer(uint32_t size, VkBufferUsageFlags usage);

void freeBuffer(MemoryId memory);

void init();
void destroy();

} // namespace age::core::deviceAlloc
