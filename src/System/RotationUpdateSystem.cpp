#include "System/RotationUpdateSystem.hpp"

#include "Globals.hpp"
#include "Component/Common.hpp"

RotationUpdateSystem::RotationUpdateSystem(void) : System("RotationUpdate")
{
    get_manager()->subscribe<Entity,RotationUpdate>(this);
}

void RotationUpdateSystem::run(float) {
    auto CM = get_manager();

    for (const UUID uuid : m_followed) {
        auto& entity = CM->get<Entity>(uuid);
        auto& update = CM->get<RotationUpdate>(uuid);

        entity.angle = rotate(entity.angle, update.dtheta);

        update.dtheta = 0.f;
    }
}
