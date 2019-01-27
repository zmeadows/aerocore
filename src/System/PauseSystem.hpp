#pragma once

#include "System.hpp"
#include "unstd/DynamicArray.hpp"
#include "Component/Common.hpp"

struct PauseSystem {
    System base = System("Pause");
    DynamicArray<UUID> finished;
    SUBSCRIBE(PauseBehavior);
};

void run(PauseSystem&, f32 dt);
