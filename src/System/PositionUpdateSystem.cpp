#include "System/PositionUpdateSystem.hpp"

#include "Engine/ComponentManager.hpp"

//TODO: factor out Extent and create separate ExtentUpdateSystem?
void PositionUpdateSystem::par_run(ComponentManager* CM, const Slice<UUID>& entities, f32 dt) {
    for (const UUID uuid : entities) {
        auto& entity = CM->get<Entity>(uuid);
        auto& update = CM->get<PositionUpdate>(uuid);

        entity.pos += update.delta;

        entity.extent.minX += update.delta.x;
        entity.extent.maxX += update.delta.x;
        entity.extent.minY += update.delta.y;
        entity.extent.maxY += update.delta.y;

        update = { 0.f, 0.f };
    }
}
