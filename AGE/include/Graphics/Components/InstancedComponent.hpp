#pragma once

#include <vector>
#include <stdint.h>
#include <sstream>

#include "Containers/DynamicBuffer.hpp"
#include "ShaderComponent.hpp"

namespace age {

template<typename T>
class Instanced {
    std::vector<T> m_instances;
    Buffer m_buffer;

public:
    const T& operator[](size_t i) const {
        return m_instances[i];
    }

    T& operator[](size_t i) {
        return m_instances[i];
    }

    inline void add(T elem) {
        m_instances.push_back(elem);
    }

    inline void erase(size_t i) {
        m_instances.erase(m_instances.begin() + i);
    }

    void upload() {
        m_buffer.load(m_instances.data(), sizeof(T) * m_instances.size());
    }

    void create(int instanceCount) {
        m_buffer.create(
            BufferCreateInfo()
            .setUsage(VK_BUFFER_USAGE_STORAGE_BUFFER_BIT)
            .setSize(sizeof(T) * instanceCount)
        );
    }

    ShaderComponentInfo getInfo() {
        ShaderComponentInfo info = T::__getInfo();
        info.m_instanced = true;
        info.m_description.m_type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        info.m_description.m_descriptor = m_buffer;
        info.setId<Instanced<T>>();
        return info;
    }
};

} // namespace age
