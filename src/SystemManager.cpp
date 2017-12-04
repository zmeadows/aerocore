#include "SystemManager.hpp"

void TranslationSystem::wrapEntity(const UUID& uuid) {

    auto pos = CM->get<Position>(uuid);
    const auto spr = CM->get<Sprite>(uuid);
    const auto rot = CM->get<Rotation>(uuid);
    const auto vel = CM->get<Velocity>(uuid);

    const Extent ext = spr->extentAt(*pos, *rot);

    if (ext.maxX > 100.0 && vel->x > 0) {
        pos->x -= (ext.maxX + 100.f);
    }

    if (ext.minX < -100.0 && vel->x < 0) {
        pos->x -= (ext.minX - 100.f);
    }

    if (ext.maxY > 100.0 && vel->y > 0) {
        pos->y -= (ext.maxY + 100.f);
    }

    if (ext.minY < -100.0 && vel->y < 0) {
        pos->y -= (ext.minY - 100.f);
    }
}

void TranslationSystem::run(float dt) {
    UUIDSet toDestroy;

    for (const UUID& uuid : m_followed) {
        auto pos = CM->get<Position>(uuid);
        auto spr = CM->get<Sprite>(uuid);
        auto rot = CM->get<Rotation>(uuid);

        bool skipUpdating = false;

        if (spr && rot) {
            if (spr->isOffScreen(*pos, *rot)) {
                auto ofb = CM->get<OffscreenBehavior>(uuid);
                if (ofb) {
                    if (*ofb == OffscreenBehavior::DiesInstantly) {
                        toDestroy.insert(uuid);
                        skipUpdating = true;
                    } else if (*ofb == OffscreenBehavior::Wraps) {
                        wrapEntity(uuid);
                    }
                }
            }
        }

        if (!skipUpdating) {
            auto const vel = CM->get<Velocity>(uuid);
            auto const acc = CM->get<Acceleration>(uuid);

            pos->x += dt * (vel->x + 0.5 * acc->x * dt);
            pos->y += dt * (vel->y + 0.5 * acc->x * dt);

            vel->x += dt * acc->x;
            vel->y += dt * acc->y;
        }
    }

    for (const UUID& uuid : toDestroy)
        CM->destroy(uuid);
}

void RotationSystem::run(float dt) {
    for (const UUID& uuid : m_followed) {
        auto rot = CM->get<Rotation>(uuid);
        auto const vel = CM->get<RotationalVelocity>(uuid);
        rot->rotateAngle(vel->value * dt);
    }
}

bool CollisionSystem::areColliding(const UUID& uuidA, const UUID& uuidB) {
    const auto allA = CM->get<Alliance>(uuidA);
    const auto allB = CM->get<Alliance>(uuidB);

    if (*allA == *allB) {
        return false;
    } else {
        DEBUG("attempting collision detection in full");
        const auto bsA = CM->get<BoundingSurface>(uuidA);
        const auto posA = CM->get<Position>(uuidA);
        const auto rotA = CM->get<Rotation>(uuidA);

        const auto bsB = CM->get<BoundingSurface>(uuidB);
        const auto posB = CM->get<Position>(uuidB);
        const auto rotB = CM->get<Rotation>(uuidB);

        return overlaps(*bsA, *posA, *rotA, *bsB, *posB, *rotB);
    }
}

void CollisionSystem::run(float dt) {
    // TODO: make a CollisionSet class for better performance
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
}