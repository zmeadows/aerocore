#include "System/CollisionSystem.hpp"

#include "Entity.hpp"

bool CollisionSystem::are_colliding(const UUID& uuidA, const UUID& uuidB) {
    if (uuidA == uuidB) return false;

    auto CM = get_manager();

    const auto& colA = CM->get<CollisionData>(uuidA);
    const auto& colB = CM->get<CollisionData>(uuidB);

    if (colA.friendly == colB.friendly) {
        return false;
    } else {
        const auto& cdA = CM->get<Entity>(uuidA);
        const auto& cdB = CM->get<Entity>(uuidB);

        return overlaps(colA, cdA, colB, cdB);
    }
}

void CollisionSystem::run(float dt) {
    std::set<std::pair<UUID, UUID>> collisions;

    auto CM = get_manager();

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
        destroy_entity(p.first);
        destroy_entity(p.second);
    }
}

