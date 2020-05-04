#include <string>
#include <fstream>

#include "utils.hpp"

namespace age {

std::string resourcePath;

void setResourcePath(const std::string& path) {
    resourcePath = path;
}

std::string getResourcePath(const std::string& resource) {
    return resourcePath + resource;
}

std::vector<char> readFile(const std::string& filename, bool relative) {
    std::string path;
    if (relative) {
        path = resourcePath + filename;
    } else {
        path = filename;
    }
    std::ifstream file(path, std::ios::ate | std::ios::binary);

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

} // namespace age

