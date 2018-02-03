#pragma once

#include "aerocore.hpp"

#include "Base.hpp"

#include <random>

template <EntityType EType>
UUID generate(ComponentManager* const CM) {
    UUID newUUID;
    return newUUID;
}

template <>
inline UUID generate<EntityType::Player>(ComponentManager* const CM) {
    CM->add<Position>(UUID::playerUUID, {0, 0});
    CM->add<Rotation>(UUID::playerUUID, Rotation());

    CM->add<Velocity>(UUID::playerUUID, {0, 0});
    CM->add<RotationalVelocity>(UUID::playerUUID, RotationalVelocity());

    CM->add<Acceleration>(UUID::playerUUID, {0, 0});

    CM->add<Alliance>(UUID::playerUUID, Alliance::Friend);
    CM->add<OffscreenBehavior>(UUID::playerUUID, OffscreenBehavior::Wraps);

    auto spr = new IsoTriangleSprite(7.0, 12.0);
    spr->rgba.r = 255;
    spr->rgba.g = 255;
    spr->rgba.b = 255;
    spr->rgba.a = 255;

    CM->add<Sprite>(UUID::playerUUID, spr);
    CM->add<BoundingSurface>(UUID::playerUUID, spr->buildBoundingSurface());

    auto sprUpd = [CM](Sprite* thisSpr) -> void {
        SimplePolygonSprite* spr = static_cast<SimplePolygonSprite*>(thisSpr);

        auto pacc = CM->get<Acceleration>(UUID::playerUUID);
        if (std::fabs(pacc->x) > 0 || std::fabs(pacc->y) > 0) {
            spr->rgba.r = std::min(255, spr->rgba.r + 3);
            spr->rgba.g = std::min(255, spr->rgba.r + 3);
            spr->rgba.b = std::min(255, spr->rgba.r + 3);
        } else {
            spr->rgba.r = std::max(100, spr->rgba.r - 3);
            spr->rgba.g = std::max(100, spr->rgba.r - 3);
            spr->rgba.b = std::max(100, spr->rgba.r - 3);
        }
    };

    CM->add<SpriteUpdator>(UUID::playerUUID, {sprUpd});

    ShotDelay sd = {1000.f, 0.0};
    CM->add<ShotDelay>(UUID::playerUUID, sd);

    auto vel = CM->get<Velocity>(UUID::playerUUID);
    vel->x = 0.f;
    vel->y = 0.f;

    auto pgen = [CM](void) -> void {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<> dis(0.0, 1.0);

        if (dis(gen) < 0.5) {
            auto pacc = CM->get<Acceleration>(UUID::playerUUID);
            if (std::fabs(pacc->x) > 0 || std::fabs(pacc->y) > 0) {
                const auto ppos = CM->get<Position>(UUID::playerUUID);
                const auto pvel = CM->get<Velocity>(UUID::playerUUID);
                const auto prot = CM->get<Rotation>(UUID::playerUUID);
                const float pangle = prot->getAngle();

                UUID partUUID;
                CM->add<Alliance>(partUUID, Alliance::Friend);
                CM->add<DeathTimer>(partUUID, {2.f});

                const float posX = ppos->x + 6.0 * std::sin(pangle) + 1.0 * dis(gen);
                const float posY = ppos->y - 6.0 * std::cos(pangle) + 1.0 * dis(gen);
                CM->add<Position>(partUUID, {posX, posY});
                CM->add<Rotation>(partUUID, Rotation());
                CM->add<Acceleration>(partUUID, Acceleration());

                const float velX = 0.25 * pvel->x + 50.0 * (0.5 - dis(gen)) - 10.0 * signum(pacc->x);
                const float velY = 0.25 * pvel->y + 50.0 * (0.5 - dis(gen)) - 10.0 * signum(pacc->y);
                CM->add<Velocity>(partUUID, {velX, velY});

                auto spr = new SquareSprite(0.5);
                spr->rgba.r = 255;
                spr->rgba.g = 255;
                spr->rgba.b = 255;
                spr->rgba.a = 120;
                CM->add<Sprite>(partUUID, spr);

                auto sprUpd = [](Sprite* thisSpr) -> void {
                    SimplePolygonSprite* spr = static_cast<SimplePolygonSprite*>(thisSpr);
                    spr->rgba.a = std::max(0, spr->rgba.a - 2);
                };

                CM->add<SpriteUpdator>(partUUID, {sprUpd});
            }
        }
    };

    CM->add<ParticleGenerator>(UUID::playerUUID, {pgen});

    return UUID::playerUUID;
}

template <>
inline UUID generate<EntityType::Bullet>(ComponentManager* const CM) {
    UUID bulletUUID;

    auto bulletSprite = new SquareSprite(1.0);
    bulletSprite->rgba.r = 0;
    bulletSprite->rgba.g = 200;
    bulletSprite->rgba.b = 0;
    bulletSprite->rgba.a = 255;

    CM->add<Sprite>(bulletUUID, bulletSprite);
    CM->add<Position>(bulletUUID, Position());
    CM->add<Rotation>(bulletUUID, Rotation());
    CM->add<Velocity>(bulletUUID, Velocity());
    CM->add<RotationalVelocity>(bulletUUID, RotationalVelocity());
    CM->add<Acceleration>(bulletUUID, {0, 0});
    CM->add<OffscreenBehavior>(bulletUUID, OffscreenBehavior::DiesInstantly);
    CM->add<BoundingSurface>(bulletUUID, bulletSprite->buildBoundingSurface());

    return bulletUUID;
}

template <>
inline UUID generate<EntityType::Enemy>(ComponentManager* const CM) {
    UUID enemyUUID;

    auto spr = new IsoTriangleSprite(10.0, 20.0);
    spr->rgba.r = 255;
    spr->rgba.a = 255;
    CM->add<Sprite>(enemyUUID, spr);

    CM->add<Position>(enemyUUID, Position());
    CM->add<Velocity>(enemyUUID, Velocity());
    CM->add<Rotation>(enemyUUID, Rotation());
    CM->add<RotationalVelocity>(enemyUUID, RotationalVelocity());
    CM->get<RotationalVelocity>(enemyUUID)->value = 0.1;
    CM->add<Acceleration>(enemyUUID, {0, 0});
    CM->add<Alliance>(enemyUUID, Alliance::Foe);
    CM->add<OffscreenBehavior>(enemyUUID, OffscreenBehavior::Wraps);

    auto pos = CM->get<Position>(enemyUUID);
    pos->x = 0.f;
    pos->y = 50.f;

    auto vel = CM->get<Velocity>(enemyUUID);
    vel->x = 0.f;
    vel->y = 40.f;

    CM->add<BoundingSurface>(enemyUUID, spr->buildBoundingSurface());

    return enemyUUID;
}
