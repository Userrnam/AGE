#pragma once

#include <string>
#include <vector>

namespace age {

template<typename... Args>
constexpr size_t getSize(Args... args) {
	return (sizeof(args) + ... + 0);
}

template<typename... Args>
constexpr size_t getSize() {
	return (sizeof(Args) + ... + 0);
}

void setResourcePath(const std::string& path);
std::string getResourcePath(const std::string& resource);
std::vector<char> readFile(const std::string& filename);

} // namespace age
