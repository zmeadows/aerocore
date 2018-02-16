#include "SystemManager.hpp"

void TranslationSystem::wrapEntity(const UUID& uuid) {
    auto& pos = CM->get<Position>(uuid);
    const auto& spr = CM->get<Sprite>(uuid);
    const auto& rot = CM->get<Rotation>(uuid);
    const auto& vel = CM->get<Velocity>(uuid);

    static Extent ext;
    extentAt(ext, spr, pos, rot);

    if (ext.maxX > 100.f && vel.x > 0.f) {
        pos.x -= (ext.maxX + 100.f);
    }

    if (ext.minX < -100.f && vel.x < 0.f) {
        pos.x -= (ext.minX - 100.f);
    }

    if (ext.maxY > 100.f && vel.y > 0.f) {
        pos.y -= (ext.maxY + 100.f);
    }

    if (ext.minY < -100.f && vel.y < 0.f) {
        pos.y -= (ext.minY - 100.f);
    }

    DEBUG("WRAPPED UUID: " << uuid);
}

void TranslationSystem::run(float dt) {
    UUIDSet toDestroy;

    for (const UUID uuid : m_followed) {
        auto& pos = CM->get<Position>(uuid);
        // auto& spr = CM->get<Sprite>(uuid);
        // auto& rot = CM->get<Rotation>(uuid);

        bool skipUpdating = false;

        // if (spr && rot) {
        //     if (spr->isOffScreen(*pos, *rot)) {
        //         auto ofb = CM->get<OffscreenBehavior>(uuid);
        //         if (ofb) {
        //             if (*ofb == OffscreenBehavior::DiesInstantly) {
        //                 toDestroy.insert(uuid);
        //                 skipUpdating = true;
        //             } else if (*ofb == OffscreenBehavior::Wraps) {
        //                 wrapEntity(uuid);
        //             }
        //         }
        //     }
        // }

        if (!skipUpdating) {
            auto& vel = CM->get<Velocity>(uuid);
            auto& acc = CM->get<Acceleration>(uuid);

            pos.x += dt * (vel.x + 0.5f * acc.x * dt);
            pos.y += dt * (vel.y + 0.5f * acc.x * dt);

            vel.x += dt * (acc.x - signum(vel.x) * 0.4f);
            vel.y += dt * (acc.y - signum(vel.y) * 0.4f);
        }
    }

    for (const UUID uuid : toDestroy)
        CM->destroy(uuid);
}

void RotationSystem::run(float dt) {
    for (const UUID uuid : m_followed) {
        auto& rot = CM->get<Rotation>(uuid);
        const auto& vel = CM->get<RotationalVelocity>(uuid);
        rot.rotateAngle(vel.value * dt);
    }
}

void ParticleSystem::run(float dt) {
    for (const UUID uuid : m_followed) {
        auto pg = CM->get<ParticleGenerator>(uuid);
        pg.generate();
    }
}

void CleanupSystem::run(float dt) {
    UUIDSet toDestroy;

    for (const UUID uuid : m_followed) {
        auto& dtmr = CM->get<DeathTimer>(uuid);
        dtmr.value -= dt;
        if (dtmr.value <= 0.f)
            toDestroy.insert(uuid);
    }

    for (const UUID uuid : toDestroy)
        CM->destroy(uuid);
}

bool CollisionSystem::areColliding(const UUID& uuidA, const UUID& uuidB) {
    return false;
    /*
    const auto allA = CM->get<Alliance>(uuidA);
    const auto allB = CM->get<Alliance>(uuidB);

    if (allA == allB) {
        return false;
    } else {
        const auto bsA = CM->get<BoundingSurface>(uuidA);
        const auto posA = CM->get<Position>(uuidA);
        const auto rotA = CM->get<Rotation>(uuidA);

        const auto bsB = CM->get<BoundingSurface>(uuidB);
        const auto posB = CM->get<Position>(uuidB);
        const auto rotB = CM->get<Rotation>(uuidB);

        return overlaps(bsA, posA, rotA, bsB, posB, rotB);
    }
    */
}

void CollisionSystem::run(float dt) {
    /*
    std::set<std::pair<UUID, UUID>> collisions;

    for (UUIDSet::iterator uuidA = m_followed.begin(); uuidA != m_followed.end(); ++uuidA) {
        for (auto uuidB = next(uuidA); uuidB != m_followed.end(); ++uuidB) {
            if (areColliding(*uuidA, *uuidB)) {
                collisions.insert(std::make_pair(*uuidA, *uuidB));
            }
        }
    }

    for (const auto& p : collisions) {
        CM->destroy(p.first);
        CM->destroy(p.second);
    }
    */
}

void DrawSystem::run(float) {
    Uint64 tmp = SDL_GetPerformanceCounter();
    m_fpsHistory[m_modFrame % 100] =
        1.0 / (static_cast<double>(tmp - m_lastFrameTicks) / static_cast<double>(SDL_GetPerformanceFrequency()));
    m_lastFrameTicks = tmp;
    m_modFrame++;

    for (const UUID uuid : m_followed) {
        auto spr = CM->get<Sprite>(uuid);

        // if (CM->has<SpriteUpdator>(uuid)) {
        //     CM->get<SpriteUpdator>(uuid).update(spr);
        // };

        draw(GC, spr, CM->get<Position>(uuid), CM->get<Rotation>(uuid));
        //TODO: add switch to turn on/off bounding surfaces with key press in debug mode.
        // auto bs = CM->get<BoundingSurface>(uuid);
        // if (bs) {
        //     bs->draw(GC, *CM->get<Position>(uuid), *CM->get<Rotation>(uuid));
        // }
    }

    stringColor(GC->renderer, 10, 10, "SCORE: 0", 0xFFFFFFFF);

    if (m_modFrame % 100 == 99) {
        double sum = 0;
        for (size_t i = 0; i < 100; i++) {
            sum += m_fpsHistory[i];
        }
        m_currentFpsAvg = sum / 100.0;
    }

    stringColor(GC->renderer, 735, 10,
                ("FPS: " + std::to_string(static_cast<int>(std::round(m_currentFpsAvg)))).c_str(),
                0xFFFFFFFF);
}
