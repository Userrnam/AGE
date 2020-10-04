#pragma once

#include <vector>
#include <sstream>

#include "ShaderComponent.hpp"
#include "StorageComponent.hpp"
#include "ComponentCollector.hpp"

namespace age {

// todo: remove this and use custom variant
static const Inserts& _getVariantInserts(const std::variant<ShaderComponentBuffer, ShaderComponentTexture, ShaderComponentForward>& v) {
    if (std::holds_alternative<ShaderComponentBuffer>(v)) {
        return std::get<ShaderComponentBuffer>(v);
    }
    if (std::holds_alternative<ShaderComponentTexture>(v)) {
        return std::get<ShaderComponentTexture>(v);
    }
    if (std::holds_alternative<ShaderComponentForward>(v)) {
        return std::get<ShaderComponentForward>(v);
    }
}

template<typename... Args>
class Bundle {
    std::tuple<Args...> m_data;
public:
    template<typename T>
    T& get() {
        return std::get<T>(m_data);
    }

    template<typename T>
    Bundle& set(const T& data) {
        std::get<T>(m_data) = data;
    }
};

template<typename... Args>
class BundleComponent : public StorageComponent<Bundle<Args...>> {
public:
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
