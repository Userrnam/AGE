#pragma once

#include <vector>
#include <sstream>

#include "ShaderComponent.hpp"
#include "StorageComponent.hpp"
#include "ComponentCollector.hpp"

#include "Containers/Tuple.hpp"

namespace age {

// todo: remove this and use custom variant
static inline const Inserts& _getVariantInserts(const std::variant<ShaderComponentBuffer, ShaderComponentTexture, ShaderComponentForward>& v) {
    if (std::holds_alternative<ShaderComponentBuffer>(v)) {
        return std::get<ShaderComponentBuffer>(v);
    }
    if (std::holds_alternative<ShaderComponentTexture>(v)) {
        return std::get<ShaderComponentTexture>(v);
    }
    if (std::holds_alternative<ShaderComponentForward>(v)) {
        return std::get<ShaderComponentForward>(v);
    }
    
    // shut up xcode
    return std::get<ShaderComponentBuffer>(v);
}

// Warning: does not call constructor
template<typename... Args>
class Bundle : public Tuple<Args...> {
public:
    template<typename T>
    Bundle& set(const T& data) {
        this->template get<T>() = data;
        return *this;
    }
};

// Warning: does not call constructor
template<typename... Args>
class BundleComponent : public StorageComponent<Bundle<Args...>> {
public:
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

    static ShaderComponentInfo __getInfo() {
        std::vector<ShaderComponentInfo> infos;
        ComponentCollector<Args...>::collect(infos);

        ShaderComponentInfo shaderComponentInfo;

        std::stringstream vertRawInsert;
        std::stringstream fragRawInsert;
        std::stringstream vertMainInsert;
        std::stringstream fragMainInsert;

        // reserve index 0 for buffer
        shaderComponentInfo.add(ShaderComponentBuffer());

        for (auto& info : infos) {
            for (auto& b : info.m_data) {
                auto& inserts = _getVariantInserts(b);
                vertRawInsert << inserts.m_vertRawInsert;
                fragRawInsert << inserts.m_fragRawInsert;
                vertMainInsert << inserts.m_vertMainInsert;
                fragMainInsert << inserts.m_fragMainInsert;
                // collect buffer members
                if (std::holds_alternative<ShaderComponentBuffer>(b)) {
                    ShaderComponentBuffer& bi = std::get<ShaderComponentBuffer>(b);
                    for (auto& member : bi.m_members) {
                        std::get<ShaderComponentBuffer>(shaderComponentInfo.m_data[0]).addBlockMember(member.m_member, member.m_forward);
                    }
                } else if (std::holds_alternative<ShaderComponentTexture>(b)) {
                    shaderComponentInfo.add(std::get<ShaderComponentTexture>(b));
                } else {
                    shaderComponentInfo.add(std::get<ShaderComponentForward>(b));
                }
            }
        }

        std::get<ShaderComponentBuffer>(shaderComponentInfo.m_data[0]).setVertRawInsert(vertRawInsert.str());
        std::get<ShaderComponentBuffer>(shaderComponentInfo.m_data[0]).setVertMainInsert(vertMainInsert.str());
        std::get<ShaderComponentBuffer>(shaderComponentInfo.m_data[0]).setFragRawInsert(fragRawInsert.str());
        std::get<ShaderComponentBuffer>(shaderComponentInfo.m_data[0]).setFragMainInsert(fragMainInsert.str());

        return shaderComponentInfo;
    }

    ShaderComponentInfo getInfo() {
        auto info = __getInfo();
        info.setId<BundleComponent<Args...>>();
        info.setBuffer(StorageComponent<Bundle<Args...>>::getBuffer());
        return info;
    }
};

} // namespace age
