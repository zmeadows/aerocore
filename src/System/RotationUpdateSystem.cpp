#include "System/RotationUpdateSystem.hpp"

#include "Globals.hpp"

void run(RotationUpdateSystem& self) {
    auto CM = get_manager();

    for (const UUID uuid : self.base.followed) {
        auto& entity = CM->get<Entity>(uuid);
        auto& update = CM->get<RotationUpdate>(uuid);

        entity.angle = rotate(entity.angle, update.dtheta);

        update.dtheta = 0.f;
    }
}
