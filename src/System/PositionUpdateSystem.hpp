#pragma once

#include "Entity.hpp"
#include "Globals.hpp"

class PositionUpdateSystem final : public System {
public:
    void run(float dt) final;

    PositionUpdateSystem(void) : System("PositionUpdate")
    {
        get_manager()->subscribe<Entity,PositionUpdate>(this);
    }
};
