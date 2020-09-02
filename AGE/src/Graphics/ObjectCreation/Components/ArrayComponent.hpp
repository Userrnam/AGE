#pragma once

#include <vector>
#include <stdint.h>
#include <sstream>

#include "Containers/DynamicBuffer.hpp"
#include "ShaderComponent.hpp"

namespace age {

template<typename T, const char* arrayIndex>
class ArrayComponent {
    std::vector<T> m_data;
    Buffer m_buffer;

public:
    const T& operator[](size_t i) const {
        return m_data[i];
    }

    T& operator[](size_t i) {
        return m_data[i];
    }

    inline void add(T elem) {
        m_data.push_back(elem);
    }

    inline void erase(size_t i) {
        m_data.erase(m_data.begin() + i);
    }

    void upload() {
        m_buffer.load(m_data.data(), sizeof(T) * m_data.size());
    }

    void destroy() {
        m_buffer.destroy();
    }

    void create(int count) {
        m_buffer.create(
            BufferCreateInfo()
            .setUsage(VK_BUFFER_USAGE_STORAGE_BUFFER_BIT)
            .setSize(sizeof(T) * count)
        );
    }

    ShaderComponentInfo getInfo() {
        ShaderComponentInfo info = T::__getInfo();
        info.m_arrayIndex = arrayIndex;
        info.m_description.m_type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        info.m_description.m_descriptor = m_buffer;
        info.setId<ArrayComponent<T, arrayIndex>>();
        return info;
    }
};

char PER_INSTANCE[] = "[gl_InstanceIndex]";
char PER_VERTEX[] = "[gl_VertexIndex]";

} // namespace age
