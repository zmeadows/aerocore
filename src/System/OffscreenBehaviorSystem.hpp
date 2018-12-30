#pragma once

#include "Entity.hpp"
#include "Globals.hpp"

class OffscreenBehaviorSystem final : public System {
public:
    void run(float dt) final;

    OffscreenBehaviorSystem(void) : System("OffscreenBehavior")
    {
        get_manager()->subscribe<Entity,OffscreenBehavior,PositionUpdate>(this);
    }
};
