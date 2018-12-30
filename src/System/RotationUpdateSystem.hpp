#pragma once

#include "Entity.hpp"
#include "Globals.hpp"

class RotationUpdateSystem final : public System {
public:
    void run(float dt) final;

    RotationUpdateSystem(void) : System("RotationUpdate")
    {
        get_manager()->subscribe<Entity,RotationUpdate>(this);
    }
};
