#include "System/EulerRotationSystem.hpp"

#include "Engine/ComponentManager.hpp"
#include "Component/Common.hpp"

void EulerRotationSystem::par_run(ComponentManager* CM, const Slice<UUID>& entities, f32 dt) {
    for (const UUID uuid : entities) {
        const auto& rot    = CM->get<EulerRotation>(uuid);
        auto& update       = CM->get<RotationUpdate>(uuid);

        update.dtheta = dt * (rot.vel + dt * rot.acc);
    }
}
