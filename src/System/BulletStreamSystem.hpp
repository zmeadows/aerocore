#pragma once

#include "System.hpp"
#include "Component/BulletStream.hpp"
#include "Component/Common.hpp"

struct BulletStreamSystem {
    System base = System("BulletStream");
    DynamicArray<UUID> finished;
    SUBSCRIBE(Entity, BulletStream);
};


void run(BulletStreamSystem&, float dt);
