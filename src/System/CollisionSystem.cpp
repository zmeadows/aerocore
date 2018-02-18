#include "System/CollisionSystem.hpp"

#include "Entity.hpp"

bool CollisionSystem::are_colliding(const UUID& uuidA, const UUID& uuidB) {
    if (uuidA == uuidB) return false;

    const auto& colA = CM->get<CollisionData>(uuidA);
    const auto& colB = CM->get<CollisionData>(uuidB);

    if (colA.friendly == colB.friendly) {
        return false;
    } else {
        const auto& cdA = CM->get<CoreData>(uuidA);
        const auto& cdB = CM->get<CoreData>(uuidB);

        if (is_offscreen(cdA.vertices, cdA.pos, cdA.angle) || is_offscreen(cdB.vertices, cdB.pos, cdB.angle))
            return false;

        const Extent extA = extent_at(cdA);
        const Extent extB = extent_at(cdB);

        if (extA.minX > extB.maxX || extB.minX > extB.maxX)
            return false;
        if (extA.minY > extB.maxY || extB.minY > extB.maxY)
            return false;

        return overlaps(colA, cdA.pos, cdA.angle, colB, cdB.pos, cdB.angle);
    }
}

void CollisionSystem::run(float dt) {
    std::set<std::pair<UUID, UUID>> collisions;

    for (UUID uuidA : m_followed) {
        const auto& colA = CM->get<CollisionData>(uuidA);
        if (!colA.node) continue;

        std::vector<UUID> collision_candidates;
        colA.node->retrieve(collision_candidates);
        for (UUID uuidB : collision_candidates) {
            if (are_colliding(uuidA, uuidB)) {
                collisions.insert(std::make_pair(uuidA, uuidB));
            }
        }
    }

    for (const auto& p : collisions) {
        destroy_entity(CM, p.first);
        destroy_entity(CM, p.second);
    }
}

