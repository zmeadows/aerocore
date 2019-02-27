#include "System/QuadTreeUpdateSystem.hpp"

#include "Engine/ComponentManager.hpp"
#include "QuadTree.hpp"

void QuadTreeUpdateSystem::par_run(ComponentManager* CM, const Slice<UUID>& entities, f32 dt) {
    for (const UUID uuid : entities) {
        auto& entity = CM->get<Entity>(uuid);
        auto& cd = CM->get<CollisionData>(uuid);

        cd.node = QT->insert_entity(uuid, clip_to_screen(entity.extent));
    }
}
void QuadTreeUpdateSystem::pre(ComponentManager*) { QT->reset(); }
