#pragma once

#include "Engine/System.hpp"

class GraphicsContext;
struct CollisionData;
struct Entity;

struct DrawCollisionGeometrySystem final : System {
    GraphicsContext* GC;
    DrawCollisionGeometrySystem(GraphicsContext* _GC)
        : GC(_GC), System("DrawCollisionGeometry") {}
    void run(ComponentManager* CM, float dt);
    SUBSCRIBE(Entity, CollisionData);
};

