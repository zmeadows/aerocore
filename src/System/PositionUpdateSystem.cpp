#include "System/PositionUpdateSystem.hpp"

#include "Globals.hpp"
#include "Component/Common.hpp"

PositionUpdateSystem::PositionUpdateSystem(void) : System("PositionUpdate")
{
    get_manager()->subscribe<Entity, PositionUpdate>(this);
}

//TODO: factor out Extent?
void PositionUpdateSystem::run(float) {
    auto CM = get_manager();

    for (const UUID uuid : m_followed) {
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
