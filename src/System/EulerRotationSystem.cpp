#include "System/EulerRotationSystem.hpp"

#include "Globals.hpp"
#include "Component/Common.hpp"

void run(EulerRotationSystem& self, f32 dt) {
    auto CM = get_manager();

    for (const UUID uuid : self.base.followed) {
        const auto& rot    = CM->get<EulerRotation>(uuid);
        auto& update       = CM->get<RotationUpdate>(uuid);

        update.dtheta = dt * (rot.vel + dt * rot.acc);
    }
}
