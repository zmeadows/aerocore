#include "System/CollisionSystem.hpp"

#include "Entity.hpp"

void CollisionSystem::run(float) {
    auto CM = get_manager();

    for (const UUID uuid : m_followed) {
        const auto& cdA = CM->get<CollisionData>(uuid);
        const auto& entityA = CM->get<Entity>(uuid);

        cdA.node->retrieve(m_candidates, uuid);

        for (UUID other_uuid : m_candidates) {
            const auto& cdB = CM->get<CollisionData>(other_uuid);
            const auto& entityB = CM->get<Entity>(other_uuid);

            if (!CM->has<FriendlyTag>(other_uuid) && overlaps(entityA, entityB, cdA, cdB)) {
                // CM->book<DestructTag>(uuid);
                CM->book<DestructTag>(other_uuid);
            }
        }
    }
}
