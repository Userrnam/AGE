#pragma once

#include <stdint.h>
#include <string>
#include <vector>

#include <glm/glm.hpp>

namespace age {

constexpr uint64_t hash(const char* str) {
    uint64_t hash = 5381;
    int c = -1;

    while ((c = *str)) {
        hash = ((hash << 4) + hash) + c;
        str++;
    }

    return hash;
}

// swapchain coordinates
// this util is created because in macos swapchain extent
// is twice bigger than window size
constexpr glm::vec2 sc(float x, float y, float m) {
    return {m * x, m * y};
}

constexpr float sc(float x, float m) {
    return m * x;
}

void setResourcePath(const std::string& path);
std::string getResourcePath(const std::string& resource);
std::vector<char> readFile(const std::string& filename);

} // namespace age
