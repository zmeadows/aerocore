#pragma once

#include "aerocore.hpp"

#include "Base.hpp"
#include "QuadTree.hpp"
#include "Globals.hpp"

class MotionSystem final : public System {
    struct HandleOffScreenBehaviorResult {
        bool skip_updating_kinematic_data = false;
        bool skip_updating_collision_data = false;
        bool to_be_destroyed = false;
    };

    HandleOffScreenBehaviorResult handle_offscreen_behavior(const UUID& uuid, Entity& cd);

public:
    void run(float dt) final;

    MotionSystem(void) : System("Motion")
    {

        get_manager()->subscribe<Entity>(this);
    }
};
