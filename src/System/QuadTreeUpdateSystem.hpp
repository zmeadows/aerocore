#pragma once

#include "System.hpp"
#include "Component/Common.hpp"
#include "Component/CollisionData.hpp"

struct QuadTreeUpdateSystem {
    System base = System("QuadTreeUpdate");
    SUBSCRIBE(Entity, CollisionData);
};

void run(QuadTreeUpdateSystem&);
