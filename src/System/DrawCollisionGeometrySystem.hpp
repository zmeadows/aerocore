#pragma once

#include "System.hpp"
#include "Component/Common.hpp"
#include "Component/CollisionData.hpp"

struct DrawCollisionGeometrySystem {
    System base = System("DrawCollisionGeometry");
    SUBSCRIBE(Entity, CollisionData);
};

void run(DrawCollisionGeometrySystem&);
