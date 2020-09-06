#pragma once

#include <vector>
#include <sstream>

#include "ShaderComponent.hpp"
#include "StorageComponent.hpp"
#include "ComponentCollector.hpp"

namespace age {

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
        auto& bufferInfo = std::get<ShaderComponentBuffer>(shaderComponentInfo.m_data[0]);

        for (auto& info : infos) {
            for (auto& b : info.m_data) {
                // collect buffer members
                if (std::holds_alternative<ShaderComponentBuffer>(b)) {
                    ShaderComponentBuffer& bi = std::get<ShaderComponentBuffer>(b);
                    for (auto& member : bi.m_members) {
                        bufferInfo.addBlockMember(member.m_member, member.m_forward);
                    }
                } else if (std::holds_alternative<ShaderComponentTexture>(b)) {
                    shaderComponentInfo.add(std::get<ShaderComponentTexture>(b));
                } else {
                    shaderComponentInfo.add(std::get<ShaderComponentForward>(b));
                }
            }
            // get inserts
            vertRawInsert << info.m_vert.rawInsert << "\n";
            fragRawInsert << info.m_frag.rawInsert << "\n";
            vertMainInsert << info.m_vert.mainInsert << "\n";
            fragMainInsert << info.m_frag.mainInsert << "\n";
        }

        shaderComponentInfo.setVertRawInsert(vertRawInsert.str());
        shaderComponentInfo.setFragRawInsert(fragRawInsert.str());
        shaderComponentInfo.setVertMainInsert(vertMainInsert.str());
        shaderComponentInfo.setFragMainInsert(fragMainInsert.str());

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
