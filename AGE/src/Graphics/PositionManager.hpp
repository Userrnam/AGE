#pragma once

#include "Math.hpp"
#include <unordered_map>
#include <iostream>

#include "../External/entt.hpp"

namespace age {

struct Positionable {
    Vector2f pos; // bottom left
    Vector2f size; // top right

    bool intersect(const Positionable& p) {
        float left = std::min(pos.x, p.pos.x);
        float right = std::max(pos.x + size.x, p.pos.x + p.size.x);
        float bottom = std::min(pos.y, p.pos.y);
        float top = std::max(pos.y + size.y, p.pos.y + p.size.y);

        return right - left < size.x + p.size.x && top - bottom < size.y + p.size.y;
    }
};

class PositionManager {
    // fixme: use something better
    static std::unordered_map<entt::entity, Positionable> m_data;
public:
    static void remove(entt::entity e) {
        m_data.erase(e);
    }

    static void update(entt::entity e, const Positionable& p) {
        m_data[e] = p;
    }

    static size_t getObjectCount() { return m_data.size(); }

    static std::vector<entt::entity> getVisibleEntities(const Positionable& camera) {
        std::vector<entt::entity> res;

        for (auto pair : m_data) {
            if (pair.second.intersect(camera)) {
                res.push_back(pair.first);
            } else {
                int k = 0;
                pair.second.intersect(camera);
            }
        }

        return res;
    }
};

} // namespace age
