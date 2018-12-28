#pragma once

#include "Base.hpp"
#include "Entity.hpp"
#include "QuadTree.hpp"
#include "Globals.hpp"

class VertexBufferSystem final : public System {
public:
    void run(float dt) final;

    VertexBufferSystem(void) : System("VertexBuffer")
    {
        get_manager()->subscribe<Entity, CollisionData>(this);
    }
};
