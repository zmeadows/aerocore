#include "System/PositionUpdateSystem.hpp"

#include "Globals.hpp"

//TODO: factor out Extent and create separate ExtentUpdateSystem?
void run(PositionUpdateSystem& self) {
    auto CM = get_manager();

    for (const UUID uuid : self.base.followed) {
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
