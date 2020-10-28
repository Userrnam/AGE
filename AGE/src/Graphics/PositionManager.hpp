#pragma once

#include "../Math/Vector.hpp"
#include <unordered_map>

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
