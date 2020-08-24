#pragma once

#include <vector>
#include <stdint.h>
#include <sstream>

#include "Containers/DynamicBuffer.hpp"
#include "ShaderComponent.hpp"

namespace age {

template <class T, class M> M __getMemberType(M T:: *);
#define GET_TYPE_OF(mem) decltype(__getMemberType(&mem))

// T's field holding data must be named m_data
// T must have a static method __getInfo() that returns ShaderComponentInfo
template<typename T>
class Instanced {
    std::vector<GET_TYPE_OF(T::m_data)> m_instances;
    Buffer m_buffer;

public:
    const GET_TYPE_OF(T::m_data)& operator[](size_t i) const {
        return m_instances[i];
    }

    GET_TYPE_OF(T::m_data)& operator[](size_t i) {
        return m_instances[i];
    }

    inline void add(GET_TYPE_OF(T::m_data) elem) {
        m_instances.push_back(elem);
    }

    inline void erase(size_t i) {
        m_instances.erase(m_instances.begin() + i);
    }

    void upload() {
        m_buffer.load(m_instances.data(), sizeof(GET_TYPE_OF(T::m_data)) * m_instances.size());
    }

    void create(int instanceCount) {
        m_buffer.create(
            BufferCreateInfo()
            .setUsage(VK_BUFFER_USAGE_STORAGE_BUFFER_BIT)
            .setSize(sizeof(GET_TYPE_OF(T::m_data)) * instanceCount)
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
