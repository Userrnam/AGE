#pragma once

#include <stdint.h>
#include <string>
#include <vector>

namespace age {

constexpr uint64_t hash(const char* str) {
    uint64_t h = 5381;
    int c = -1;

    while ((c = *str)) {
        h = ((h << 4) + h) + c;
        str++;
    }

    return h;
}

void setAssetsPath(const std::string& path);
std::string getAssetPath(const std::string& resource);
std::vector<char> readFile(const std::string& filename);

int random(int min, int max);

inline float frandom(float min, float max, float acc=10.0f) {
    return (float)random(min * acc, max * acc) / acc; 
}

} // namespace age
