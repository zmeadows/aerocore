#include "System/CollisionSystem.hpp"

#include "Entity.hpp"

void CollisionSystem::run(float) {
    auto CM = get_manager();

    for (const UUID uuid : m_followed) {
        const auto& cdA = CM->get<CollisionData>(uuid);
        const auto& extA = CM->get<Entity>(uuid).extent;

        cdA.node->retrieve(m_candidates, uuid);

        for (UUID other_uuid : m_candidates) {
            const auto& cdB = CM->get<CollisionData>(other_uuid);
            const auto& extB = CM->get<Entity>(other_uuid).extent;

            if (!CM->has<FriendlyTag>(other_uuid) && overlaps(cdA, cdB, extA, extB)) {
                //CM->book<DestructTag>(uuid);
                //CM->book<DestructTag>(other_uuid);
            }
        }
    }
}
