#pragma once

#include "Base.hpp"
#include "ComponentManager.hpp"

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

    auto vel = CM->get<Velocity>(UUID::playerUUID);
    vel->x = 0.f;
    vel->y = 0.f;

    return UUID::playerUUID;
}

template <>
inline UUID generate<EntityType::Bullet>(ComponentManager* const CM) {
    UUID bulletUUID;

    auto bulletSprite = new IsoTriangleSprite(2.0, 2.0);
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
