#include "SystemManager.hpp"

void ParticleSystem::run(float) {
    auto CM = get_manager();

    for (const UUID uuid : m_followed) {
        auto pg = CM->get<ParticleGenerator>(uuid);
        pg.generate();
    }
}

void CleanupSystem::run(float dt) {
    UUIDSet toDestroy;

    auto CM = get_manager();

    for (const UUID uuid : m_followed) {
        auto& dtmr = CM->get<DeathTimer>(uuid);
        dtmr.value -= dt;
        if (dtmr.value <= 0.f)
            toDestroy.insert(uuid);
    }

    for (const UUID uuid : toDestroy)
        CM->destroy(uuid);
}

