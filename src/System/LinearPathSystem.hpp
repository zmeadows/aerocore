#pragma once

#include "Entity.hpp"
#include "Globals.hpp"

class LinearPathSystem final : public System {
public:
    void run(float dt) final;

    LinearPathSystem(void) : System("LinearPath")
    {
        get_manager()->subscribe<Entity,LinearPath,PositionUpdate>(this);
    }
};
