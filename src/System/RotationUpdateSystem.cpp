#include "System/RotationUpdateSystem.hpp"

#include "Engine/ComponentManager.hpp"

void RotationUpdateSystem::par_run(ComponentManager* CM, const Slice<UUID>& entities, f32 dt) {
    for (const UUID uuid : entities) {
        auto& entity = CM->get<Entity>(uuid);
        auto& update = CM->get<RotationUpdate>(uuid);

        entity.angle = rotate(entity.angle, update.dtheta);
        update.dtheta = 0.f;
    }
}
