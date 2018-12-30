#include "System/RotationUpdateSystem.hpp"

#include "Entity.hpp"

void RotationUpdateSystem::run(float) {
    ComponentManager* CM = get_manager();

    for (const UUID uuid : m_followed) {
        auto& entity = CM->get<Entity>(uuid);
        auto& update = CM->get<RotationUpdate>(uuid);

        entity.angle = rotate(entity.angle, update.dtheta);

        update.dtheta = 0.f;
    }
}
