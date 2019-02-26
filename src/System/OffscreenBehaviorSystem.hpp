#pragma once

#include "Engine/System.hpp"
#include "Component/Common.hpp"
#include "Component/OffscreenBehavior.hpp"

struct OffscreenBehaviorSystem final : System {
    OffscreenBehaviorSystem() : System("OffscreenBehavior") {}
    void run(ComponentManager* CM, float dt);
    SUBSCRIBE(Entity, OffscreenBehavior, PositionUpdate);
};

