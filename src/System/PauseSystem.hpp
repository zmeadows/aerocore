#pragma once

#include "Engine/System.hpp"
#include "unstd/DynamicArray.hpp"

struct PauseBehavior;

struct PauseSystem final : System {
    PauseSystem() : System("Pause") {}
    void run(ComponentManager* CM, float dt);
    DynamicArray<UUID> finished;
    SUBSCRIBE(PauseBehavior);
};

