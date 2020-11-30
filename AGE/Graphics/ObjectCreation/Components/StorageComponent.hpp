#pragma once

#include "../../MemoryHolders/Buffer.hpp"
#include "ShaderComponent.hpp"
#include "Bundle.hpp"

namespace age {

template<typename... T>
class StorageComponent;

template<typename T>
class StorageComponent<T> {
    Buffer m_buffer;
    T m_data;

public:
    inline ShaderComponentInfo getInfo() const {
        auto info = T::__getInfo();
        info.setBuffer(m_buffer);
        info. template setId<StorageComponent<T>>();
        return info;
    }

    inline Buffer& getBuffer() {
        return m_buffer;
    }

    inline void create() {
        m_buffer.create(
            UniformBufferCreateInfo()
            .setSize(sizeof(T))
        );
        // upload default value
        set(m_data);
    }

    inline void create(const T& data) {
        create();
        set(data);
    }

    inline void destroy() {
        m_buffer.destroy();
    }

    inline T& get() {
        return m_data;
    }

    inline const T& get() const {
        return m_data;
    }

    inline void set(const T& data) {
        m_data = data;
        upload();
    }

    inline void upload() {
        m_buffer.load(&m_data, sizeof(T));
    }
};

template<typename... Args>
struct StorageComponent : public StorageComponent<Bundle<Args...>> {
    Bundle<Args...>& get() {
        return StorageComponent<Bundle<Args...>>::get();
    }

    const Bundle<Args...>& get() const {
        return StorageComponent<Bundle<Args...>>::get();
    }

    void set(const Bundle<Args...>& b) {
        StorageComponent<Bundle<Args...>>::set(b);
    }

    template<typename T>
    T& get() {
        return StorageComponent<Bundle<Args...>>::get().template get<T>();
    }

    template<typename T>
    const T& get() const {
        return StorageComponent<Bundle<Args...>>::get().template get<T>();
    }

    template<typename T>
    Bundle<Args...>& set(const T& data) {
        return StorageComponent<Bundle<Args...>>::get().set(data);
    }
};

} // namespace age
