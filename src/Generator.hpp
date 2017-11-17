#pragma once

#include "ComponentManager.hpp"

template <EntityType EType>
UUID generate(ComponentManager* const CM) {
    UUID newUUID;
    return newUUID;
}

template<>
UUID generate<EntityType::Player>(ComponentManager* const CM) {
    CM->add<Position>(UUID::playerUUID, {50,50});
    CM->add<Velocity>(UUID::playerUUID, {0,0});
    CM->add<Sprite>(UUID::playerUUID, new SquareSprite(50));
    return UUID::playerUUID;
}

template<>
UUID generate<EntityType::Bullet>(ComponentManager* const CM) {
    UUID bulletUUID;

    auto bulletSprite = new SquareSprite(25);
    bulletSprite->rgba.r = 200;
    bulletSprite->rgba.g = 200;
    bulletSprite->rgba.b = 200;

    CM->add<Sprite>(bulletUUID, bulletSprite);
    return bulletUUID;
}