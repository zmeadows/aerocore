#include "System/EulerRotationSystem.hpp"

#include "Globals.hpp"
#include "Component/Common.hpp"

EulerRotationSystem::EulerRotationSystem(void) : System("EulerRotation")
{
    get_manager()->subscribe<Entity, EulerRotation, RotationUpdate>(this);
}

void EulerRotationSystem::run(float dt) {
    ComponentManager* CM = get_manager();

    for (const UUID uuid : m_followed) {
        const auto& rot    = CM->get<EulerRotation>(uuid);
        auto& update       = CM->get<RotationUpdate>(uuid);

        update.dtheta = dt * (rot.vel + dt * rot.acc);
    }
}
