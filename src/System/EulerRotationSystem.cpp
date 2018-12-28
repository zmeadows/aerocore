#include "System/EulerRotationSystem.hpp"

#include "Entity.hpp"

void EulerRotationSystem::run(float dt) {
    static ComponentManager* CM = get_manager();

    for (const UUID uuid : m_followed) {
        const auto& entity = CM->get<Entity>(uuid);
        const auto& rot    = CM->get<EulerRotation>(uuid);
        auto& update       = CM->get<RotationUpdate>(uuid);

        update.dtheta = dt * (rot.vel + dt * rot.acc);
    }
}
