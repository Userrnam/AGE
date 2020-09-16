#pragma once

#include <stdint.h>
#include <string>
#include <vector>

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

void setResourcePath(const std::string& path);
std::string getResourcePath(const std::string& resource);
std::vector<char> readFile(const std::string& filename);

} // namespace age
