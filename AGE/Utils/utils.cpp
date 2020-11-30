#include <string>
#include <fstream>
#include <stdlib.h>

#include "utils.hpp"

namespace age {

std::string resourcePath;

void setAssetsPath(const std::string& path) {
    resourcePath = path;
}

std::string getAssetPath(const std::string& resource) {
    return resourcePath + resource;
}

std::vector<char> readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file named " + filename);
    }

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}

int random(int min, int max) {
    if (min == max) {
        return min;
    }
    return min + rand() % (max - min);
}

} // namespace age

