#pragma once

#include "aerocore.hpp"

#include "Base.hpp"
#include "Sprite.hpp"

#include <random>

template <EntityType EType>
UUID generate(ComponentManager* const CM) {
    UUID newUUID;
    return newUUID;
}

template <>
inline UUID generate<EntityType::Player>(ComponentManager* const CM) {
    CM->book<Position>(playerUUID());
    CM->book<Rotation>(playerUUID());
    CM->book<Velocity>(playerUUID());
    CM->book<RotationalVelocity>(playerUUID());
    CM->book<Acceleration>(playerUUID());

    CM->book<Alliance>(playerUUID(), Alliance::Friend);
    CM->book<OffscreenBehavior>(playerUUID(), OffscreenBehavior::Wraps);

    auto spr = CM->book<NewSprite>(playerUUID(), makeIsoTriangleSprite(7.0, 12.0));
    spr.color.r = 255;
    spr.color.g = 255;
    spr.color.b = 255;
    spr.color.a = 255;

    // CM->book<BoundingSurface*>(playerUUID(), spr->buildBoundingSurface());

    // auto sprUpd = [CM](Sprite* thisSpr) -> void {
    //     SimplePolygonSprite* spr = static_cast<SimplePolygonSprite*>(thisSpr);

    //     auto pacc = CM->get<Acceleration>(playerUUID());
    //     if (std::fabs(pacc.x) > 0 || std::fabs(pacc.y) > 0) {
    //         spr->rgba.r = std::min(255, spr->rgba.r + 3);
    //         spr->rgba.g = std::min(255, spr->rgba.r + 3);
    //         spr->rgba.b = std::min(255, spr->rgba.r + 3);
    //     } else {
    //         spr->rgba.r = std::max(100, spr->rgba.r - 3);
    //         spr->rgba.g = std::max(100, spr->rgba.r - 3);
    //         spr->rgba.b = std::max(100, spr->rgba.r - 3);
    //     }
    // };

    // CM->book<SpriteUpdator>(playerUUID(), {sprUpd});

    auto sd = CM->book<ShotDelay>(playerUUID());
    sd.delay = 1000.f;

    /*
    auto pgen = [CM](void) -> void {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<> dis(0.0, 1.0);

        if (dis(gen) < 0.5) {
            auto pacc = CM->get<Acceleration>(playerUUID());
            if (std::fabs(pacc.x) > 0 || std::fabs(pacc.y) > 0) {
                const auto ppos = CM->get<Position>(playerUUID());
                const auto pvel = CM->get<Velocity>(playerUUID());
                const auto prot = CM->get<Rotation>(playerUUID());
                const float pangle = prot.getAngle();

                UUID partUUID;
                CM->book<Alliance>(partUUID, Alliance::Friend);
                auto dt = CM->book<DeathTimer>(partUUID);
                dt.value = 2.0;


                auto& newpos = CM->book<Position>(partUUID);
                newpos.x = ppos.x + 6.0 * std::sin(pangle) + 1.0 * dis(gen);
                newpos.y = ppos.y - 6.0 * std::cos(pangle) + 1.0 * dis(gen);

                CM->book<Rotation>(partUUID);
                CM->book<Acceleration>(partUUID);

                auto& newvel = CM->book<Velocity>(partUUID);
                newvel.x =  0.25 * pvel.x + 50.0 * (0.5 - dis(gen)) - 10.0 * signum(pacc.x);
                newvel.y =  0.25 * pvel.y + 50.0 * (0.5 - dis(gen)) - 10.0 * signum(pacc.y);

                // auto spr = new SquareSprite(0.5);
                // spr->rgba.r = 255;
                // spr->rgba.g = 255;
                // spr->rgba.b = 255;
                // spr->rgba.a = 120;
                // CM->book<Sprite*>(partUUID, spr);

                // auto sprUpd = [](Sprite* thisSpr) -> void {
                //     SimplePolygonSprite* spr = static_cast<SimplePolygonSprite*>(thisSpr);
                //     spr->rgba.a = std::max(0, spr->rgba.a - 2);
                // };

                // CM->book<SpriteUpdator>(partUUID, {sprUpd});
            }
        }
    };
    */

    // CM->book<ParticleGenerator>(playerUUID(), pgen);

    return playerUUID();
}

template <>
inline UUID generate<EntityType::Bullet>(ComponentManager* const CM) {
    UUID bulletUUID;

    auto bulletSprite = CM->book<NewSprite>(bulletUUID, makeSquareSprite(1.0));
    bulletSprite.color.r = 0;
    bulletSprite.color.g = 200;
    bulletSprite.color.b = 0;
    bulletSprite.color.a = 255;

    // CM->book<Sprite>(bulletUUID, bulletSprite);
    // CM->book<BoundingSurface>(bulletUUID, bulletSprite->buildBoundingSurface());
    CM->book<Position>(bulletUUID);
    CM->book<Rotation>(bulletUUID);
    CM->book<Velocity>(bulletUUID);
    CM->book<RotationalVelocity>(bulletUUID);
    CM->book<Acceleration>(bulletUUID);
    CM->book<OffscreenBehavior>(bulletUUID, OffscreenBehavior::DiesInstantly);

    return bulletUUID;
}

template <>
inline UUID generate<EntityType::Enemy>(ComponentManager* const CM) {
    UUID enemyUUID;

    auto spr = CM->book<NewSprite>(enemyUUID, makeIsoTriangleSprite(10.0, 20.0));
    spr.color.r = 255;
    spr.color.g = 255;
    spr.color.b = 255;
    spr.color.a = 255;

    // CM->book<BoundingSurface>(enemyUUID, spr->buildBoundingSurface());

    auto& pos = CM->book<Position>(enemyUUID);
    pos.y = 50.0;

    auto& vel = CM->book<Velocity>(enemyUUID);
    vel.y = 40.0;

    CM->book<Acceleration>(enemyUUID);

    CM->book<Rotation>(enemyUUID);
    auto& rot = CM->book<RotationalVelocity>(enemyUUID);
    rot.value = 0.1;

    CM->book<Alliance>(enemyUUID, Alliance::Foe);
    CM->book<OffscreenBehavior>(enemyUUID, OffscreenBehavior::Wraps);

    return enemyUUID;
}
