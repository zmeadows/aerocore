#pragma once

#include "System.hpp"
#include "Component/Common.hpp"

struct StateTransitionCleanupSystem {
    System base = System("StateTransitionCleanup");
    DynamicArray<UUID> followed_copy;
    SUBSCRIBE(StateTransition);
};

void run(StateTransitionCleanupSystem&);
