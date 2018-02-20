#pragma once

#include "aerocore.hpp"

#include "System.hpp"
#include "Base.hpp"
#include "BoundingSurface.hpp"
#include "Globals.hpp"

class CollisionSystem final : public System {
    bool are_colliding(const UUID& uuidA, const UUID& uuidB);

public:
    void run(float dt) final;

    CollisionSystem(void) : System("Collision") {
        get_manager()->subscribe<Entity, CollisionData>(this);
    }
};
