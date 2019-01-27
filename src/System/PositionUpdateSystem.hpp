#pragma once

#include "System.hpp"
#include "Component/Common.hpp"

struct PositionUpdateSystem {
    System base = System("PositionUpdate");
    SUBSCRIBE(Entity, PositionUpdate);
};

void run(PositionUpdateSystem&);
