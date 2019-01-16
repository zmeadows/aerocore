#include "System/CollisionSystem.hpp"

#include "Entity.hpp"

#include "Brofiler.h"

void CollisionSystem::run(float) {

	BROFILER_CATEGORY("CollisionSystem", Profiler::Color::Orchid)

    auto CM = get_manager();

    for (const UUID uuid : m_followed) {
        const auto& cdA = CM->get<CollisionData>(uuid);
        const auto& entityA = CM->get<Entity>(uuid);

        cdA.node->retrieve(m_candidates, uuid);

        for (const UUID other_uuid : m_candidates) {
            const auto& cdB = CM->get<CollisionData>(other_uuid);
            const auto& entityB = CM->get<Entity>(other_uuid);

            //BUGFIX: hack of requiring no DestructTag (rare bug)
            if (!CM->has<FriendlyTag>(other_uuid) && !CM->has<DestructTag>(other_uuid) && overlaps(entityA, entityB, cdA, cdB)) {
                CM->book<DestructTag>(other_uuid);
                //CM->book<DestructTag>(uuid);
            }
        }
    }
}
