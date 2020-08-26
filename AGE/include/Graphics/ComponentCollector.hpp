#pragma once

#include <vector>

#include "Components/ShaderComponent.hpp"

namespace age {

template<typename... Args>
void collectComponents(std::vector<ShaderComponentInfo>& components, Args... args);

template<typename Head, typename...Tail>
void collectComponents(std::vector<ShaderComponentInfo>& components, Head head, Tail... tail) {
    components.push_back(head.getInfo());
    collectComponents(components, tail...);
}

template<typename Head>
void collectComponents(std::vector<ShaderComponentInfo>& components, Head head) {
    components.push_back(head.getInfo());
}

} // namespace age
