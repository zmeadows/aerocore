#include "System/EulerTranslationSystem.hpp"

#include "Entity.hpp"

void EulerTranslationSystem::run(float dt) {
    ComponentManager* CM = get_manager();

    for (const UUID uuid : m_followed) {
        auto& kin    = CM->get<EulerTranslation>(uuid);
        auto& update = CM->get<PositionUpdate>(uuid);

        update.delta.x = dt * (kin.vel.x + 0.5f * kin.acc.x * dt);
        update.delta.y = dt * (kin.vel.y + 0.5f * kin.acc.y * dt);

        kin.vel.x += dt * (kin.acc.x - signum(kin.vel.x) * kin.drag);
        kin.vel.y += dt * (kin.acc.y - signum(kin.vel.y) * kin.drag);
    }
}
