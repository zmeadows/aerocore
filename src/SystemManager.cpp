#include "SystemManager.hpp"

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

