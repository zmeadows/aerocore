#pragma once

#include "System.hpp"
#include "Component/Common.hpp"
#include "Component/OffscreenBehavior.hpp"

struct OffscreenBehaviorSystem {
    System base = System("OffscreenBehavior");
    SUBSCRIBE(Entity, OffscreenBehavior, PositionUpdate);
};

void run(OffscreenBehaviorSystem&);
