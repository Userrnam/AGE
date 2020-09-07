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

protected:
    inline Buffer getBuffer() { return m_buffer; }

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

    inline void clear() {
        m_data.clear();
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

    ShaderComponentInfo __getInfo() {
        auto info = T::__getInfo();
        assert(std::holds_alternative<ShaderComponentBuffer>(info.m_data[0]));
        std::get<ShaderComponentBuffer>(info.m_data[0]).m_arrayIndex = arrayIndex;
        return info;
    }

    ShaderComponentInfo getInfo() {
        auto info = __getInfo();
        info.template setId<ArrayComponent<T, arrayIndex>>();
        info.setBuffer(m_buffer);
        return info;
    }
};

const char PER_INSTANCE[] = "[gl_InstanceIndex]";
const char PER_VERTEX[] = "[gl_VertexIndex]";

} // namespace age
