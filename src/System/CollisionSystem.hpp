#pragma once

#include "System.hpp"
#include "unstd/DynamicArray.hpp"
#include "Component/Common.hpp"
#include "Component/CollisionData.hpp"

struct CollisionSystem {
    System base = System("Collision");
    DynamicArray<UUID> collision_candidates;
    SUBSCRIBE(Entity, CollisionData, FriendlyTag);
};

void run(CollisionSystem&);
