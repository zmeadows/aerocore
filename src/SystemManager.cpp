#include "SystemManager.hpp"

void TranslationSystem::run(float dt)
{
    for (const UUID& uuid : m_followed) {
        auto pos = CM->get<Position>(uuid);
        auto const vel = CM->get<Velocity>(uuid);
        auto const acc = CM->get<Acceleration>(uuid);

        pos->x += dt * vel->x;
        pos->y += dt * vel->y;
        vel->x += dt * acc->x;
        vel->y += dt * acc->y;

        // if (uuid.unwrap() == UUID::playerUUID.unwrap()) {
        //     if (acc->x == 0.0 && std::abs(vel->x) < 10.0)
        //         vel->x *= 0.9;
        //     if (acc->y == 0.0 && std::abs(vel->y) < 10.0)
        //         vel->y *= 0.9;
        // }
    }
}

void RotationSystem::run(float dt)
{
    for (const UUID& uuid : m_followed) {
        auto rot = CM->get<Rotation>(uuid);
        auto const vel = CM->get<RotationalVelocity>(uuid);
        rot->rotateAngle(vel->value * dt);
    }
}

bool CollisionSystem::areColliding(const UUID& uuidA, const UUID& uuidB)
{
    const auto allA = CM->get<Alliance>(uuidA);
    const auto allB = CM->get<Alliance>(uuidB);

    if (*allA == *allB) {
        return false;
    } else {
        const auto bsA = CM->get<BoundingSurface>(uuidA);
        const auto posA = CM->get<Position>(uuidA);
        const auto rotA = CM->get<Rotation>(uuidA);

        const auto bsB = CM->get<BoundingSurface>(uuidB);
        const auto posB = CM->get<Position>(uuidB);
        const auto rotB = CM->get<Rotation>(uuidB);

        return overlaps(*bsA, *posA, *rotA, *bsB, *posB, *rotB);
    }
}

void CollisionSystem::run(float dt)
{
    std::set<std::pair<UUID, UUID>> collisions;

    for (std::set<UUID>::const_iterator uuidA = m_followed.begin(); uuidA != m_followed.end(); ++uuidA) {
        for (std::set<UUID>::const_iterator uuidB = next(uuidA); uuidB != m_followed.end(); ++uuidB) {
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