#pragma once

#include "aerocore.hpp"

#include "Base.hpp"
#include "BoundingSurface.hpp"
#include "Enemy.hpp"
#include "Globals.hpp"
#include "System.hpp"

//@TODO: no need for inheritance.
class CollisionSystem final : public System {
public:
    void run(float dt) final;

    CollisionSystem(void) : System("Collision") {
        get_manager()->subscribe<Entity>(this);
    }
};
