#pragma once

#include "System.hpp"
#include "Component/Common.hpp"

struct EulerRotationSystem {
    System base = System("EulerRotation");
    SUBSCRIBE(Entity, EulerRotation, RotationUpdate);
};

void run(EulerRotationSystem&, f32 dt);
