#pragma once

#include "System.hpp"
#include "Component/Common.hpp"

struct RotationUpdateSystem {
    System base = System("RotationUpdate");
    SUBSCRIBE(Entity, RotationUpdate);
};

void run(RotationUpdateSystem&);
