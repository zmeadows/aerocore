#pragma once

#include "Base.hpp"
#include "ComponentManager.hpp"

#include <random>

template <EntityType EType>
UUID generate(ComponentManager* const CM) {
    UUID newUUID;
    return newUUID;
}

template <>
inline UUID generate<EntityType::Player>(ComponentManager* const CM) {
    /*
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

        auto pacc = CM->get<Acceleration>(UUID::playerUUID);
        if (std::fabs(pacc->x) > 0 || std::fabs(pacc->y) > 0) {
            const auto ppos = CM->get<Position>(UUID::playerUUID);
            const auto pvel = CM->get<Velocity>(UUID::playerUUID);
            const auto prot = CM->get<Rotation>(UUID::playerUUID);
            const float pangle = prot->getAngle();
            for (size_t i = 0; i < 10; i++) {
                UUID partUUID;
                CM->add<Alliance>(partUUID, Alliance::Friend);
                CM->add<DeathTimer>(partUUID, {0.5f});

                const float posX = ppos->x + 5.0 * std::sin(pangle) + 1.0 * (0.5 - dis(gen));
                const float posY = ppos->y - 5.0 * std::cos(pangle) + 1.0 * (0.5 - dis(gen));
                CM->add<Position>(partUUID, {posX, posY});
                CM->add<Rotation>(partUUID, Rotation());
                CM->add<Acceleration>(partUUID, Acceleration());

                const float velX = 0.25 * pvel->x + 50.0 * (0.5 - dis(gen)) - 10.0 * signum(pacc->x);
                const float velY = 0.25 * pvel->y + 50.0 * (0.5 - dis(gen)) - 10.0 * signum(pacc->y);
                CM->add<Velocity>(partUUID, {velX, velY});

                auto spr = new SquareSprite(0.3);
                spr->rgba.r = 200;
                spr->rgba.g = 200;
                spr->rgba.b = 0;
                spr->rgba.a = 50;
                CM->add<Sprite>(partUUID, spr);

                auto sprUpd = [](Sprite* thisSpr) -> void {
                    SimplePolygonSprite* spr = static_cast<SimplePolygonSprite*>(thisSpr);
                    spr->rgba.a = std::max(0, spr->rgba.a - 1);
                };

                CM->add<SpriteUpdator>(partUUID, {sprUpd});
            }
        }
    };

    CM->add<ParticleGenerator>(UUID::playerUUID, {pgen});

    */
    return UUID::playerUUID;
}

template <>
inline UUID generate<EntityType::Bullet>(ComponentManager* const CM) {
    UUID bulletUUID;

    /*
    SquareSprite* bulletSprite = new (CM->book<Sprite>(bulletUUID)) SquareSprite(1.0);
    bulletSprite->rgba.r = 200;
    bulletSprite->rgba.g = 200;
    bulletSprite->rgba.b = 200;
    bulletSprite->rgba.a = 255;

    new (CM->book<Position>(bulletUUID)) Position();
    new (CM->book<Rotation>(bulletUUID)) Rotation();
    new (CM->book<Velocity>(bulletUUID)) Velocity();
    new (CM->book<RotationalVelocity>(bulletUUID)) RotationalVelocity();

    new (CM->book<Acceleration>(bulletUUID)) Acceleration();

    new (CM->book<OffscreenBehavior>(bulletUUID))  OffscreenBehavior::DiesInstantly);

    CM->add<BoundingSurface>(bulletUUID, *bulletSprite->buildBoundingSurface());
    */

    return bulletUUID;
}

template <>
inline UUID generate<EntityType::Enemy>(ComponentManager* const CM) {
    UUID enemyUUID;

    /*
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
        */

    return enemyUUID;
}
