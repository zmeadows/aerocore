#pragma once

#include "aerocore.hpp"

#include "Base.hpp"
#include "Entity.hpp"
#include "QuadTree.hpp"
#include "Globals.hpp"

class MotionSystem final : public System {
    bool handle_offscreen_behavior(const UUID& uuid, Entity& cd);

public:
    void run(float dt) final;

    MotionSystem(void) : System("Motion")
    {

        get_manager()->subscribe<Entity>(this);
    }
};
