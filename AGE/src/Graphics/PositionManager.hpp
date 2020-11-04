#pragma once

#include "../Math/Vector.hpp"
#include "../Math/Rectangle.hpp"

#include <unordered_map>

#include "../External/entt.hpp"

namespace age {

typedef Rectangle Positionable;

class PositionManager {
    // fixme: use something better
    std::unordered_map<entt::entity, Positionable> m_data;
public:
    void remove(entt::entity e) { m_data.erase(e); }
    void update(entt::entity e, const Positionable& p) { m_data[e] = p; }

    size_t getObjectCount() { return m_data.size(); }

    std::vector<entt::entity> getVisibleEntities(const Positionable& camera) {
        std::vector<entt::entity> res;

        for (auto pair : m_data) {
            if (pair.second.intersect(camera)) {
                res.push_back(pair.first);
            } else {
                pair.second.intersect(camera);
            }
        }

        return res;
    }
};

} // namespace age
