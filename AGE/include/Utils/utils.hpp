#pragma once

#include <string>
#include <vector>

namespace age {

void setResourcePath(const std::string& path);
std::string getResourcePath(const std::string& resource);
std::vector<char> readFile(const std::string& filename);

} // namespace age
