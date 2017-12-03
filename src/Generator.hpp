#pragma once

#include "Base.hpp"
#include "ComponentManager.hpp"

template <EntityType EType>
UUID generate(ComponentManager* const CM)
{
    UUID newUUID;
    return newUUID;
}

template <>
inline UUID generate<EntityType::Player>(ComponentManager* const CM)
{
    CM->add<Position>(UUID::playerUUID, {0, 0});
    CM->add<Rotation>(UUID::playerUUID, Rotation());

    CM->add<Velocity>(UUID::playerUUID, {0, 0});
    CM->add<RotationalVelocity>(UUID::playerUUID, RotationalVelocity());

    CM->add<Acceleration>(UUID::playerUUID, {0, 0});

    CM->add<Alliance>(UUID::playerUUID, Alliance::Friend);

    auto spr = new IsoTriangleSprite(7.0, 12.0);
    spr->rgba.r = 255;
    spr->rgba.g = 255;
    spr->rgba.b = 255;
    spr->rgba.a = 255;

    CM->add<Sprite>(UUID::playerUUID, spr);
    CM->add<BoundingSurface>(UUID::playerUUID, spr->buildBoundingSurface());

    return UUID::playerUUID;
}

template <>
inline UUID generate<EntityType::Bullet>(ComponentManager* const CM)
{
    UUID bulletUUID;

    auto bulletSprite = new SquareSprite(2);
    bulletSprite->rgba.r = 0;
    bulletSprite->rgba.g = 200;
    bulletSprite->rgba.b = 0;
    bulletSprite->rgba.a = 255;
    CM->add<Sprite>(bulletUUID, bulletSprite);

    CM->add<Position>(bulletUUID, Position());
    CM->add<Rotation>(bulletUUID, Rotation());
    CM->add<Velocity>(bulletUUID, {0, 10});
    CM->add<RotationalVelocity>(bulletUUID, RotationalVelocity());
    CM->get<RotationalVelocity>(bulletUUID)->value = 100;
    CM->add<Acceleration>(bulletUUID, {0, 0});

    CM->add<BoundingSurface>(bulletUUID, bulletSprite->buildBoundingSurface());

    return bulletUUID;
}

template <>
inline UUID generate<EntityType::Enemy>(ComponentManager* const CM)
{
    UUID enemyUUID;
    CM->add<Position>(enemyUUID, {0, 75});
    CM->add<Rotation>(enemyUUID, Rotation());
    CM->add<Velocity>(enemyUUID, {0, 0});
    CM->add<RotationalVelocity>(enemyUUID, RotationalVelocity());
    CM->add<Acceleration>(enemyUUID, {0, 0});

    auto spr = new SquareSprite(10.0);
    spr->rgba.r = 255;
    spr->rgba.a = 255;
    CM->add<Sprite>(enemyUUID, spr);
    CM->add<Alliance>(enemyUUID, Alliance::Foe);

    CM->add<BoundingSurface>(enemyUUID, spr->buildBoundingSurface());

    return enemyUUID;
}
