#include "System/CollisionSystem.hpp"

#include "Entity.hpp"

bool CollisionSystem::are_colliding(const UUID& uuidA, const UUID& uuidB) {
    const auto& colA = CM->get<CollisionData>(uuidA);
    const auto& colB = CM->get<CollisionData>(uuidB);

    if (colA.friendly == colB.friendly) {
        return false;
    } else {
        const auto& cdA = CM->get<CoreData>(uuidA);
        const auto& cdB = CM->get<CoreData>(uuidB);

        if (is_offscreen(cdA.vertices, cdA.pos, cdA.angle) || is_offscreen(cdB.vertices, cdB.pos, cdB.angle))
            return false;

        return overlaps(colA, cdA.pos, cdA.angle, colB, cdB.pos, cdB.angle);
    }
}

void CollisionSystem::run(float dt) {
    std::set<std::pair<UUID, UUID>> collisions;

    for (UUIDSet::iterator uuidA = m_followed.begin(); uuidA != m_followed.end(); ++uuidA) {
        for (auto uuidB = next(uuidA); uuidB != m_followed.end(); ++uuidB) {
            if (are_colliding(*uuidA, *uuidB)) {
                collisions.insert(std::make_pair(*uuidA, *uuidB));
            }
        }
    }

    for (const auto& p : collisions) {
        destroy_entity(CM, p.first);
        destroy_entity(CM, p.second);
    }
}

