#pragma once

#include <vector>

#include "ShaderComponent.hpp"

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

template<typename... Args>
struct ComponentCollector;

template<typename Head, typename... Tail>
struct ComponentCollector<Head, Tail...> {
    static void collect(std::vector<ShaderComponentInfo>& components) {
        components.push_back(Head::__getInfo());
        ComponentCollector<Tail...>::collect(components);
    }
};

template<typename Head>
struct ComponentCollector<Head> {
    static void collect(std::vector<ShaderComponentInfo>& components) {
        components.push_back(Head::__getInfo());
    }
};

} // namespace age
