#pragma once

#include "ComponentManager.hpp"

template <EntityType EType> UUID generate(ComponentManager* const CM) {
    UUID newUUID;
    return newUUID;
}

template <> UUID generate<EntityType::Player>(ComponentManager* const CM) {
    CM->add<Position>(UUID::playerUUID, {0, 0});
    CM->add<Velocity>(UUID::playerUUID, {0, 0});
    CM->add<Alliance>(UUID::playerUUID, Alliance::Friend);

    CM->add<Sprite>(UUID::playerUUID, new SquareSprite(10.0));
    auto const spr = CM->get<Sprite>(UUID::playerUUID);
    CM->add<BoundingSurface>(UUID::playerUUID, spr->buildBoundingSurface());

    return UUID::playerUUID;
}

template <> UUID generate<EntityType::Bullet>(ComponentManager* const CM) {
    UUID bulletUUID;

    // auto bulletSprite = new SquareSprite(25);
    // bulletSprite->rgba.r = 0;
    // bulletSprite->rgba.g = 200;
    // bulletSprite->rgba.b = 0;

    // CM->add<Sprite>(bulletUUID, bulletSprite);

    // CM->add<Position>(bulletUUID, Position());

    // auto const pos = CM->get<Position>(bulletUUID);
    // CM->add<BoundingSurface>(bulletUUID, new BoundingBox(25, pos));

    return bulletUUID;
}

template <> UUID generate<EntityType::Enemy>(ComponentManager* const CM) {
    UUID enemyUUID;
    CM->add<Position>(enemyUUID, {75, 0});
    CM->add<Velocity>(enemyUUID, {0, 0});

    auto spr = new SquareSprite(10.0);
    spr->rgba.r = 255;
    spr->rgba.g = 0;
    CM->add<Sprite>(enemyUUID, spr);
    CM->add<Alliance>(enemyUUID, Alliance::Foe);

    CM->add<BoundingSurface>(enemyUUID, spr->buildBoundingSurface());

    return enemyUUID;
}
