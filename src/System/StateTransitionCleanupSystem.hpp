#pragma once

#include "Engine/System.hpp"
#include "Component/Common.hpp"

struct StateTransitionCleanupSystem final : System {
    StateTransitionCleanupSystem() : System("StateTransitionCleanup") {}
    void run(ComponentManager* CM, float dt);
    DynamicArray<UUID> followed_copy;
    SUBSCRIBE(StateTransition);
};

