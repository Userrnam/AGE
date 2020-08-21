#pragma once

#include "../Buffer.hpp"

namespace age {

template<typename T>
class StorageComponent {
    T m_data;
    Buffer m_buffer;

public:
    inline Buffer& getBuffer() {
        return m_buffer;
    }

    inline void create() {
        m_buffer.create(
            UniformBufferCreateInfo()
            .setSize(sizeof(T))
        );
    }

    inline void destroy() {
        m_buffer.destroy();
    }

    inline void upload() {
        m_buffer.load(&m_data, sizeof(T));
    }

    inline T& get() {
        return m_data;
    }

    inline const T& get() const {
        return m_data;
    }

    inline void set(T data) {
        m_data = data;
    }
};

} // namespace age